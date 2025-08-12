#include "exchange.h"
#include "stock.h"
#include <chrono>
#include <deque>
#include <memory> // for std::unique_ptr, std::make_unique
#include <omp.h>
#include <queue>
#include <string>  // for std::string
#include <utility> // for std::move
#include <vector>

// TODO: Account for overflow in times
Exchange::Exchange(void)
    : market_data{}, live_stocks{}, pending_trades{},
      market_start_time(Exchange::getCurrentTime()),
      time_since_market_start(Exchange::getCurrentTime()) {}

long long Exchange::getCurrentTime(void) const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch()).count();
}

long long Exchange::initMarketStartTime() { return getCurrentTime(); }

const std::unordered_map<std::string, std::deque<StockFrame>> &
Exchange::getMarketData(void) const {
  return market_data;
}

long long Exchange::getExchangeStartTime(void) const {
  return market_start_time;
}

long long Exchange::getTimeSinceExchangeStart(void) const {
  return Exchange::getCurrentTime() - market_start_time;
}

std::vector<std::string> Exchange::getLiveStocks() const {
  std::vector<std::string> tickers = getKeys(live_stocks);
  return tickers;
}
double Exchange::getStockPrice(std::string ticker) const {
  // Defaults to -1 if the stock is not live
  auto it = live_stocks.find(ticker);
  if (it != live_stocks.end() && it->second != nullptr) {
    return it->second->getCurrPrice();
  }
  return -1.0; // stock not found
}

void Exchange::updateMarketData(StockFrame sf) {
  // Market Data contains that stock
  // Consider changing this to string_view for speedup
  std::string ticker = static_cast<std::string>(sf.ticker);
  market_data[ticker].push_back(sf);
}

int Exchange::addStock(Stock *stock) {
  std::string ticker_str = static_cast<std::string>(stock->getTicker());

  live_stocks[ticker_str] = stock;

  // Initialize market_data with first StockFrame
  std::deque<StockFrame> stockList;
  stockList.push_back(stock->toStockFrame());
  market_data[ticker_str] = stockList;

  return 0;
}

void Exchange::printMarketData(void) const {
  for (const auto &[ticker, sfll] : market_data) {
    std::cout << "Ticker: " << ticker << "\n"
              << "Stock Frame List: " << sfll;
  }
}

void Exchange::printLatestMarketDataframe(void) const {
  for (const auto &[ticker, sfll] : market_data) {
    std::cout << sfll.back();
  }
}

MarketDataframe Exchange::getMarketSnapshot(int recent_count = 50) const {
  MarketDataframe snapshot = {};

  for (const auto &[ticker, stock_frames] : market_data) {
    std::deque<StockFrame> recent_frames;

    if (stock_frames.size() <= static_cast<long unsigned int>(recent_count)) {
      // Convert deque to list
      recent_frames.assign(stock_frames.begin(), stock_frames.end());
    } else {
      // Get last 'recent_count' elements
      auto start_it = stock_frames.end() - recent_count;
      recent_frames.assign(start_it, stock_frames.end());
    }
    snapshot.sf_ll[ticker] = std::move(recent_frames);
  }

  return snapshot;
}

void Exchange::collectTrade(Trader_X_Msg &txm) {
  auto &trader = txm.trader;
  auto &msg = txm.msg;

  // You cannot sell a stock that you don't own
  if (msg.trade_type == Trades::MARKET_SELL ||
      msg.trade_type == Trades::LIMIT_SELL) {
    if (trader->getStockQuantity(msg.ticker) < msg.quantity) {
      return;
    }
  }

  // Can't trade a stock that isn't live
  if (!live_stocks.contains(msg.ticker)) {
    return;
  } else {
    live_stocks[msg.ticker]->pushOrder(txm);
  }
}

void Exchange::matchOrders() {
  // Collect the pointers to stocks into a vector to allow for parallel
  // iteration. This is necessary because unordered_map iterators are not
  // designed for parallel modification.
  std::vector<Stock *> stocks_to_process;
  stocks_to_process.reserve(live_stocks.size());
  for (const auto &[ticker, stock] : live_stocks) {
    stocks_to_process.push_back(stock);
  }
// OpenMP parallelizes the loop over the vector of stocks.
// Each thread processes a different stock's order book.
#pragma omp parallel for
  for (size_t i = 0; i < stocks_to_process.size(); ++i) {
    Stock *stock = stocks_to_process[i];

    auto &buy_book = stock->getBuyBook();
    auto &sell_book = stock->getSellBook();

    double acc_stock_price = 0;
    long int acc_stock_volume = 0;

    while (!buy_book.empty() && !sell_book.empty()) {
      const Trader_X_Msg &buy_order = buy_book.top();
      const Trader_X_Msg &sell_order = sell_book.top();

      const auto &buy_order_msg = buy_order.msg;
      const auto &sell_order_msg = sell_order.msg;

      bool buy_is_market = (buy_order_msg.trade_type == Trades::MARKET_BUY);
      bool sell_is_market = (sell_order_msg.trade_type == Trades::MARKET_SELL);

      // If it is a limit order, and prices don't mactch, break
      if (!buy_is_market && !sell_is_market &&
          buy_order_msg.price < sell_order_msg.price) {
        break;
      }

      // TODO can improve this to have more volume traded
      int order_volume =
          std::min(buy_order_msg.quantity, sell_order_msg.quantity);

      double order_price;
      // If both are market orders, trade based on currStockprice from previous
      // iteration. This translates to an institution buying you out Otherwise,
      // trade with someone that placed a limit order
      if (buy_is_market && sell_is_market) {
        order_price = stock->getCurrPrice(); // fallback midpoint
      } else if (buy_is_market ||
                 sell_order_msg.trade_type == Trades::LIMIT_SELL) {
        order_price = sell_order_msg.price;
      } else {
        order_price = buy_order_msg.price;
      }

      // If the buyer cannot afford the order, the order is removed from the
      // heap
      // TODO check if you want to change this so that the volume you get
      // decreases if you can't afford it
      if (buy_order.trader->getCashBalance() < order_price * order_volume) {
        buy_book.pop();
        continue;
      }
      // Execute the trade
      sell_order.trader->updateCashBalance(order_price * order_volume);
      buy_order.trader->updateCashBalance(-order_price * order_volume);
      sell_order.trader->updateStockHoldings(
          static_cast<std::string>(stock->getTicker()), -order_volume);
      buy_order.trader->updateStockHoldings(
          static_cast<std::string>(stock->getTicker()), order_volume);

      acc_stock_price += order_price * order_volume;
      acc_stock_volume += order_volume;

      // Handle partial fills by popping the original and pushing the updated
      // copy.
      buy_book.pop();
      if (buy_order_msg.quantity > order_volume) {
        Trader_X_Msg remaining_buy = buy_order; // Make a mutable copy
        remaining_buy.msg.quantity -= order_volume;
        buy_book.push(remaining_buy);
      }

      // Handle sell side
      sell_book.pop();
      if (sell_order_msg.quantity > order_volume) {
        Trader_X_Msg remaining_sell = sell_order; // Make a mutable copy
        remaining_sell.msg.quantity -= order_volume;
        sell_book.push(remaining_sell);
      }
    }
    if (acc_stock_volume > 0) {
      stock->setCurrPrice(acc_stock_price / acc_stock_volume);
    }
    stock->setVolTraded(stock->getVolTraded() + acc_stock_volume);
  }
}

void Exchange::simulateTick(const std::vector<Trader *> &traders) {
  MarketDataframe snapshot = getMarketSnapshot();

  for (Trader *trader : traders) {
    Trades::Trade_Message msg = trader->submitTrade(snapshot);
    Trader_X_Msg tr_x_msg = {trader, msg};
    trader->incrementTicksHeld();
    collectTrade(tr_x_msg);
  }

  matchOrders(); // apply price/volume changes

  // Update market_data with new snapshots
  for (auto &[ticker, stock] : live_stocks) {
    updateMarketData(stock->toStockFrame());
  }

  ++time_since_market_start;
}
