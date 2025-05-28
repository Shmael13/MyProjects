#pragma once
#include <unorderedset>
#include "stock_frame.cpp"
#include "stock_frame_linked_list.h"

struct MarketDataframe{
  std::unordered_map<std::string, StockFrameLL> sf_ll;
    
}
