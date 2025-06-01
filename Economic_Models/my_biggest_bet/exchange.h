#pragma once
#include <unordered_map>
#include "utils/stock_frame_linked_list.h"

class Exchange{
  private:
    std::unordered_map<std::string, std::unique_ptr<StockFrameLL>> market_data;  //contains all the market data so far - stockframes for each timestep.
    static long long market_start_time;
    long long time_since_market_start;
    static long long getCurrentTime(void);
    static long long initMarketStartTime();

  public:
    Exchange(void);
    std::unordered_map<std::string, std::unique_ptr<StockFrameLL>> getMarketData(void) const;
    static long long getExchangeStartTime(void);
    long long getTimeSinceExchangeStart(void) const;
    void updateMarketData(std::string ticker, StockFrame sf);
};
