#pragma once
#include <random>
#include "utils/trade_message.cpp"


class Trader{
  private:
    long long trader_id;
    std::function<trade_message(market_dataframe)> trade_func;
}

