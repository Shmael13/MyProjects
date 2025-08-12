#include "stock.h"
#include "exchange.h"
#include "utils/industry.h"
#include <chrono>

Stock::Stock(std::string_view ticker, std::shared_ptr<Industry> industryPtr,
             double curr_price, long int num_stocks, Exchange &exchange)
    : ticker(ticker), industry(industryPtr), curr_price(curr_price),
      vol_traded(0), num_stocks(num_stocks),
      exchange_start_time(exchange.getExchangeStartTime()) {
  industry->addStock(this);
}

// Getter Methods
std::string_view Stock::getIndustryName() const { return industry->getName(); }

std::shared_ptr<Industry> Stock::getIndustry() const { return industry; }

std::string_view Stock::getTicker() const { return ticker; }

double Stock::getCurrPrice() const { return curr_price; }

long int Stock::getVolTraded() const { return vol_traded; }

long int Stock::getNumStocks() const { return num_stocks; }

// Setter Methods
void Stock::setCurrPrice(double curr_price) { this->curr_price = curr_price; }

void Stock::setVolTraded(long int vol_traded) { this->vol_traded = vol_traded; }

void Stock::setNumStocks(long int num_stocks) { this->num_stocks = num_stocks; }

long long Stock::getCurrTime() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now()
                 .time_since_epoch() // change this to time since exchange
                                     // started once the exchange is implemented
             )
      .count();
}

void Stock::pushOrder(Trader_X_Msg txm) {
  auto trade_type = txm.msg.trade_type;
  if (trade_type == Trades::MARKET_BUY || trade_type == Trades::LIMIT_BUY) {
    buy_book.push(txm);
  } else {
    sell_book.push(txm);
  }
}

StockFrame Stock::toStockFrame(void) const {
  return StockFrame{ticker,     industry->getName(),
                    curr_price, vol_traded,
                    num_stocks, getCurrTime() - exchange_start_time};
}
