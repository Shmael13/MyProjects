#pragma once
#include <iostream>
#include <string>

namespace Trades {

enum TradeType {
  MARKET_BUY = 0,
  LIMIT_BUY = 1,
  MARKET_SELL = 2,
  LIMIT_SELL = 3,
};

struct Trade_Message {
  std::string ticker;
  TradeType trade_type;
  double price;
  int quantity;

  
  Trade_Message(
    std::string ticker = "",
    TradeType trade_type = MARKET_BUY,
    double price = 0,
    int quantity = 0)
    :  
    ticker(ticker),
    trade_type(trade_type),
    price(price),
    quantity(quantity)
  {if (ticker == ""){
    std::cerr << "UNINITILIZED TRADE_MESSAGE";
  }}


};
} // namespace Trades

constexpr std::ostream &operator<<(std::ostream &out,
                                   Trades::Trade_Message tm) {
  out << "\tTicker:         " << tm.ticker << "\n"
      << "\tTrade_Type:     " << tm.trade_type << "\n"
      << "\tPrice:          " << tm.price << "\n"
      << "\tQuantity:       " << tm.quantity << "\n";
  return out;
}
