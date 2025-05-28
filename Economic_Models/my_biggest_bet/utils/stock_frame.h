#pragma once
#include <string>

struct StockFrame{
  std::string ticker;
  std::string industry_name;
  double curr_price;
  long int vol_traded;
  long int num_stocks;
  long long recording_time;
};


