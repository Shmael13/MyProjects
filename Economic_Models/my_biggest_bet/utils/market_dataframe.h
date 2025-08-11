#pragma once

#include "stock_frame.h"

struct MarketDataframe{
  std::unordered_map<std::string, std::deque<StockFrame>> sf_ll;
};
