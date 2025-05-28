#pragma once
#include <string>
#include <memory>
#include "utils/stock_frame.h"

class Industry;

class Stock{
  private:
    std::string ticker;
    std::shared_ptr<Industry> industry;
    double curr_price;
    long int vol_traded;
    long int num_stocks;
    long long getCurrTime() const;

  public:
  //Init
  Stock(std::string ticker, std::shared_ptr<Industry> industryPtr, double curr_price, long int num_stocks);
  
  //Attribute Getter Methods
  std::string getIndustryName() const;
  std::shared_ptr<Industry> getIndustry() const;
  std::string getTicker() const;
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
