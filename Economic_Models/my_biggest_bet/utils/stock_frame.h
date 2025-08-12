#pragma once
#include <deque>
#include <iostream>
#include <string>

struct StockFrame {
  std::string_view ticker;
  std::string_view industry_name;
  double curr_price;
  long int vol_traded;
  long int num_stocks;
  long long recording_time;
};

constexpr std::ostream &operator<<(std::ostream &out, StockFrame sf) {
  out << "\tTicker:         " << sf.ticker << "\n"
      << "\tIndustry:       " << sf.industry_name << "\n"
      << "\tPrice:          " << sf.curr_price << "\n"
      << "\tVolume Traded:  " << sf.vol_traded << "\n"
      << "\tNumber Stocks:  " << sf.num_stocks << "\n"
      << "\tRecording Time: " << sf.recording_time << "\n";
  return out;
}

constexpr std::ostream &operator<<(std::ostream &out,
                                   const std::deque<StockFrame> sfll) {
  out << "\nPrinting Stock Frame List:\n";
  int i = 1;
  for (const auto &sf : sfll) {
    out << "Frame #" << i << ": "
        << "\n"
        << sf << "\n";
    ++i;
  }
  return out;
}
