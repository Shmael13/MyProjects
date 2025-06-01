#include "exchange.h"
#include <chrono>
//TODO: Account for overflow in times

Exchange::Exchange(void) : 
  market_data{},
  time_since_market_start(Exchange::getCurrentTime())
{}

long long Exchange::market_start_time = Exchange::initMarketStartTime();

long long Exchange::getCurrentTime(void){
    return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

long long Exchange::initMarketStartTime() {
    return getCurrentTime();
}

std::unordered_map<std::string, std::unique_ptr<StockFrameLL>> Exchange::getMarketData(void) const{
  return market_data;
}


long long Exchange::getExchangeStartTime(void){
  return market_start_time; 
}

long long Exchange::getTimeSinceExchangeStart(void) const{
  return Exchange::getCurrentTime() - market_start_time;
}

void Exchange::updateMarketData(std::string ticker, StockFrame sf){
  //Market Data contains that stock
  if (!market_data.contains(ticker)){
    market_data[ticker] = insert_node(sf, nullptr);
  }
  else{
    market_data[ticker] = insert_node(sf, std::move(market_data[ticker]));
  }
}
