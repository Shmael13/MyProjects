#pragma once
#include <string>

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

