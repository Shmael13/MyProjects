#pragma once
#include <string>
#include <iostream>

namespace Trades {

  enum TradeType { MARKET_BUY = 0,
                   LIMIT_BUY = 1,
                   MARKET_SELL = 2,
                   LIMIT_SELL = 3,
                  };

  struct Trade_Message {
    std::string ticker;
    TradeType trade_type;
    double price;
    int quantity;
  };
}

constexpr std::ostream& operator<<(std::ostream& out, Trades::Trade_Message tm){
  out << "\tTicker:         " << tm.ticker          << "\n"
      << "\tTrade_Type:     " << tm.trade_type      << "\n"
      << "\tPrice:          " << tm.price           << "\n"
      << "\tQuantity:       " << tm.quantity        << "\n";
  return out;
}

