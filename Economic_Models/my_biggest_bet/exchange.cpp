#include "exchange.h"
#include <chrono>

long long Exchange::getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

Exchange::Exchange(void){
  market_start_time = Exchange::getCurrentTime();
}

market_data Exchange::getMarketData(void) const{
  return market_data;
}


long long Exchange::getMarketStartTime(void) const{
  return market_start_time; 
}

long long Exchange::getTimeSinceMarketStart(void) const{
  return Exchange::getCurrentTime() - market_start_time;
}


