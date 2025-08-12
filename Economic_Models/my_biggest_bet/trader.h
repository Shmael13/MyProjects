#pragma once
#include "utils/market_dataframe.h"
#include "utils/trade_message.h"
#include <functional>
#include <random>
// TODO trader can currently send only one Trade_Message i.e a trader can only
// trade one stock per tick right now. Change this later.

class Trader {
private:
  std::string trader_id;
  std::function<Trades::Trade_Message(const MarketDataframe &)> trade_func;
  double cash_balance;
  std::unordered_map<std::string, int> stock_holdings;
  int ticks_held = 0;

public:
  // Constructor
  Trader(std::string id,
         std::function<Trades::Trade_Message(const MarketDataframe &)> func,
         double initial_cash = 1e6)
      : trader_id(std::move(id)), trade_func(std::move(func)),
        cash_balance(std::max(initial_cash, 0.0)), stock_holdings() {}

  // Generate trade based on market data snapshot
  Trades::Trade_Message submitTrade(const MarketDataframe &market) {
    return trade_func(market);
  }

  // Accessor
  std::string getTraderId() const { return trader_id; }

  // Tick tracker
  void incrementTicksHeld() { ticks_held++; }

  void resetTicksHeld() { ticks_held = 0; }
  int getTicksHeld() const { return ticks_held; }

  const auto getStockHoldings() const { return stock_holdings; }
  int getStockQuantity(const std::string &ticker) {
    auto iterator = stock_holdings.find(ticker);
    return iterator != stock_holdings.end() ? iterator->second : 0;
  }
  double getCashBalance() const { return cash_balance; }

  void updateStockHoldings(const std::string &ticker, int quantity) {
    stock_holdings[ticker] += quantity;
    if (stock_holdings[ticker] <= 0) {
      stock_holdings.erase(ticker);
    }
  }

  // Expects +ive input for gaining cash, -ive input for losing cash
  void updateCashBalance(double quantity) { cash_balance += quantity; }
};

class PyTrader : public Trader {
private:
  std::string module_name;
  std::string function_name;

public:
  PyTrader(std::string id,
           std::function<Trades::Trade_Message(const MarketDataframe &)> func,
           std::string module_name, std::string function_name,
           double initial_cash = 1e6)
      : Trader(std::move(id), std::move(func),
               initial_cash), // call base class constructor
        module_name(std::move(module_name)),
        function_name(std::move(function_name)) {}
};
