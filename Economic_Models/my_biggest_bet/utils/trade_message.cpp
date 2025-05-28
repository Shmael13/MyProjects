#pragma once

enum class TradeType {BUY, SELL};

struct Trade_Message{
  std::string trader_id;
  TradeType trade_type;
  double price;
  long quantity;
}
