#pragma once
#include <random>
#include "utils/trade_message.cpp"
#include "utils/stock_frame_linked_list.h"

class Trader{
  private:
    long long trader_id;
    std::function<trade_message(market_dataframe)> trade_func;
  `
}

