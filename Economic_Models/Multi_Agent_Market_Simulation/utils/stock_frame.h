#pragma once
#include <deque>
#include <iostream>
#include <string>
#include <chrono>

struct StockFrame {
  std::string_view ticker;
  std::string_view industry_name;
  double curr_price;
  long int vol_traded;
  long int num_stocks;
  long int recording_time;



  // Included initializer for -Weffc++ warning
  StockFrame(
      std::string_view ticker = "",
      std::string_view industry_name = "",
      double price = 0.0,
      long int volume_traded = 0,
      long int num_stocks = 0,
      // The line below tells how much time has passed since the start (1982 or smth) in milliseconds
      long int time =   std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch()).count()
      )
      : ticker(std::move(ticker)),
        industry_name(std::move(industry_name)),
        curr_price(price),
        vol_traded(volume_traded),
        num_stocks(num_stocks),
        recording_time(time)
  {if (ticker == ""){
    std::cerr << "UNINITILIZED STOCKFRAME";
    }}
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
