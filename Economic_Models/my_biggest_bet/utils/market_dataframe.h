#pragma once

#include "stock_frame.h"

struct MarketDataframe{
  std::unordered_map<std::string, std::list<StockFrame>> sf_ll;
};
