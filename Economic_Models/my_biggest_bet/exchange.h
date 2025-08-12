#pragma once
#include "trader.h"
#include "utils/market_dataframe.h"
#include "utils/trade_message.h"
#include <unordered_map>
#include <vector>

// Used within getLiveStocks
template <typename K, typename V>
std::vector<K> getKeys(const std::unordered_map<K, V> &map) {
  std::vector<K> keys;
  keys.reserve(map.size()); // optional optimization
  for (const auto &pair : map) {
    keys.push_back(pair.first);
  }
  return keys;
}

class Stock;

struct Trader_X_Msg {
  Trader *trader;
  Trades::Trade_Message msg;
};

class Exchange {
private:
  std::unordered_map<std::string, std::deque<StockFrame>>
      market_data; // contains all the market data so far - stockframes for each
                   // timestep.

  std::unordered_map<std::string, Stock *> live_stocks;
  std::vector<Trader_X_Msg> pending_trades;

  long long market_start_time;
  long long time_since_market_start;
  long long getCurrentTime(void) const;
  long long initMarketStartTime();

public:
  Exchange(void);
  const std::unordered_map<std::string, std::deque<StockFrame>> &
  getMarketData(void) const;
  long long getExchangeStartTime(void) const;
  long long getTimeSinceExchangeStart(void) const;
  void updateMarketData(StockFrame sf);
  int addStock(Stock *stock);
  void printMarketData(void) const;
  std::vector<std::string> getLiveStocks() const;
  double getStockPrice(std::string ticker) const;
  void printLatestMarketDataframe(void) const;

  // Methods for trader interaction
  MarketDataframe getMarketSnapshot(int recent_count) const;
  void collectTrade(Trader_X_Msg &msg);
  void matchOrders(); // Simplified version
  void simulateTick(const std::vector<Trader *> &traders);
};
