#include "py_binder.h"

namespace Trade_Algos {
// Static flag to track Python initialization
   // auto og_pytrader = [](const MarketDataframe& market) -> Trades::Trade_Message {
   //     // Only initialize once
   //     PyBinder::initialize_python();
   //     
   //     // Don't finalize here - let cleanup_python() handle it
   //     Trades::Trade_Message trade = PyBinder::getTradeMessageFromPython(
   //         "python_trader",
   //         "make_trade_decision", 
   //         market
   //     );
   //     
   //     return trade;
   // };

   // auto moving_avg_pytrader = [](const MarketDataframe& market) -> Trades::Trade_Message {
   //     // Only initialize once
   //     PyBinder::initialize_python();
   //     
   //     // Don't finalize here - let cleanup_python() handle it
   //     Trades::Trade_Message trade = PyBinder::getTradeMessageFromPython(
   //         "moving_avg_trader",
   //         "make_trade_decision", 
   //         market
   //     );
   //     
   //     return trade;
   // };

   // auto neural_net_trader = [](const MarketDataframe& market) -> Trades::Trade_Message {
   //     // Only initialize once
   //     PyBinder::initialize_python();
   //     
   //     // Don't finalize here - let cleanup_python() handle it
   //     Trades::Trade_Message trade = PyBinder::getTradeMessageFromPython(
   //         "neural_net_trader",
   //         "make_trade_decision", 
   //         market
   //     );
   //     
   //     return trade;
   // };
  
  auto random_trader = [](const MarketDataframe& market) -> Trades::Trade_Message {
      const auto& stock_list = market.sf_ll.begin()->second;
      const auto& latest_frame = stock_list.back();

      // Picks first stock randomly and always buys 10 shares at current price
      std::uniform_int_distribution order_type {0, 3};
      std::uniform_int_distribution num_stocks {0, 1};
      
      int upper = static_cast<int>(latest_frame.curr_price * 1.1);
      int lower = static_cast<int>(latest_frame.curr_price * 0.9);
      lower = 10;
      if (upper < lower) std::swap(upper, lower);  // ensure valid range
      std::uniform_int_distribution<int> stock_price(lower, upper);   
      
      if (market.sf_ll.empty()) {
          return {"", static_cast<Trades::TradeType>(0), 0.0, 0};  // no-op
      }

      return {
          std::string(latest_frame.ticker),
          static_cast<Trades::TradeType>(order_type(Random::mt)),
          static_cast<double>(stock_price(Random::mt)),
          num_stocks(Random::mt)
      };
  };

  auto market_sell_trader = [](const MarketDataframe& market) -> Trades::Trade_Message {
      // Picks first stock randomly and always buys 10 shares at current price
      if (market.sf_ll.empty()) {
          return {"", static_cast<Trades::TradeType>(0), 100.0, 3};  // no-op
      }

      const auto& stock_list = market.sf_ll.begin()->second;
      const auto& latest_frame = stock_list.back();

      return {
          std::string(latest_frame.ticker),
          Trades::MARKET_SELL,
          0,
          10
      };
  };

  auto market_buy_trader = [](const MarketDataframe& market) -> Trades::Trade_Message {
      // Picks first stock randomly and always buys 10 shares at current price
      if (market.sf_ll.empty()) {
          return {"", static_cast<Trades::TradeType>(0), 0.0, 0};  // no-op
      }

      const auto& stock_list = market.sf_ll.begin()->second;
      const auto& latest_frame = stock_list.back();

      return {
          std::string(latest_frame.ticker),
          Trades::MARKET_BUY,
          0,
          10
      };
  };

  auto limit_buy_trader = [](const MarketDataframe& market) -> Trades::Trade_Message {
      // Picks first stock randomly and always buys 10 shares at current price
      if (market.sf_ll.empty()) {
          return {"", static_cast<Trades::TradeType>(0), 0.0, 0};  // no-op
      }

      const auto& stock_list = market.sf_ll.begin()->second;
      const auto& latest_frame = stock_list.back();

      return {
          std::string(latest_frame.ticker),
          Trades::LIMIT_BUY,
          40,
          1
      };
  };

  auto smart_trader = [](const MarketDataframe& market) -> Trades::Trade_Message {
      if (market.sf_ll.empty()) {
          return {"", Trades::LIMIT_BUY, 0.0, 0};  // no-op
      }

      const auto& [ticker, stock_list] = *market.sf_ll.begin();
      if (stock_list.size() < 3) {
          return {"", Trades::LIMIT_BUY, 0.0, 0};  // not enough data to determine trend
      }

      auto it = stock_list.rbegin();
      double curr = it->curr_price;
      double prev1 = (++it)->curr_price;
      double prev2 = (++it)->curr_price;

      // Calculate trend: upward, downward, or flat
      bool is_uptrend = curr > prev1 && prev1 > prev2;
      bool is_downtrend = curr < prev1 && prev1 < prev2;

      int quantity = 5;
      double price = curr;

      if (is_uptrend) {
          // Buy before price climbs further
          price = curr * 1.01;
          return {ticker, Trades::LIMIT_BUY, price, quantity};
      } else if (is_downtrend) {
          // Sell before it falls more
          price = curr * 0.99;
          return {ticker, Trades::LIMIT_SELL, price, quantity};
      } else {
          // No trade — too uncertain
          return {"", Trades::LIMIT_BUY, 0.0, 0};  // no-op
      }
  };
}



