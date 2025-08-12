#pragma once

#include "stock_frame.h"
#include <unordered_map>

struct MarketDataframe {
  std::unordered_map<std::string, std::deque<StockFrame>> sf_ll;
};
