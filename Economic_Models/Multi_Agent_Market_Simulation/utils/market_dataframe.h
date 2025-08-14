#pragma once

#include <unordered_map>
#include "stock_frame.h"

struct MarketDataframe {
  // Stands for stock frane linked list - an earlier implementation
  // TODO change this sf_ll to a better name
  std::unordered_map<std::string, std::deque<StockFrame>> sf_ll;

  MarketDataframe(
    std::unordered_map<std::string, std::deque<StockFrame>> sfll = {}
    )
    :  
    sf_ll(sfll)
  {
    if (sf_ll.empty()){
      std::cerr << "UNINITILIZED MARKET_DATAFRAME";
    }
  }
};
