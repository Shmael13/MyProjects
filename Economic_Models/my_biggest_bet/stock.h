#pragma once
#include <string>
#include <memory>
#include "utils/stock_frame.h"
#include "exchange.h"

class Industry;

class Stock{
  private:
    std::string_view ticker;
    std::shared_ptr<Industry> industry;
    double curr_price;
    long int vol_traded;
    long int num_stocks;
    long long getCurrTime() const;
    long long exchange_start_time;

  public:
  //Init
  Stock(std::string_view ticker, std::shared_ptr<Industry> industryPtr, 
               double curr_price, long int num_stocks, Exchange& exchange);
  
  //Attribute Getter Methods
  std::string_view getIndustryName() const;
  std::shared_ptr<Industry> getIndustry() const;
  std::string_view getTicker() const;
  double getCurrPrice() const;
  long int getVolTraded() const;
  long int getNumStocks() const;

  //Attribute Setter Methods
  void setCurrPrice(double curr_price);
  void setVolTraded(long int vol_traded);
  void setNumStocks(long int num_stocks);

  //Snapshot Getter Method
  StockFrame toStockFrame(void) const;
};


constexpr std::ostream& operator<<(std::ostream& out, Stock stock){
  out << "\n"
      <<  stock.getTicker() << "\n"
      << "\tIndustry:      " << stock.getIndustryName()  << "\n"
      << "\tPrice:    "    << stock.getCurrPrice()     << "\n"
      << "\tVolume Traded: "  << stock.getVolTraded()     << "\n"
      << "\tNumber Stocks: "  << stock.getNumStocks()     << "\n";
  return out;
}


