#pragma once
#include <unordered_map>
#include "utils/stock_frame_linked_list.cpp"

class Exchange{
  private:
    std::unordered_map<std::string, StockFrameLL> market_data;  //contains all the market data so far - stockframes for each timestep.
    long long market_start_time;
    long long time_since_market_start;
    long long getCurrentTime(void) const;

  public:
    Exchange(void);
    market_data getMarketData(void) const;
    long long getMarketStartTime(void) const;
    long long getTimeSinceMarketStart(void) const;
};
