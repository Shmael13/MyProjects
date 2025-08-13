#pragma once

#include <unordered_map>
#include "stock_frame.h"

struct MarketDataframe {
  std::unordered_map<std::string, std::deque<StockFrame>> sf_ll;
};
