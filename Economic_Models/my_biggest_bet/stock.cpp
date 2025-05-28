#include "stock.h"
#include "utils/industry.h"
#include <chrono>

Stock::Stock(std::string ticker, std::shared_ptr<Industry> industryPtr, double curr_price, long int num_stocks)
    :ticker(ticker), industry(industryPtr), curr_price(curr_price), vol_traded(0), num_stocks(num_stocks){
    industry->addStock(this);
}

//Getter Methods
std::string Stock::getIndustryName() const{
    return industry->getName();
}

std::shared_ptr<Industry> Stock::getIndustry() const{
    return industry;
}

std::string Stock::getTicker() const{
    return ticker;
}

double Stock::getCurrPrice() const{
    return curr_price;
}

long int Stock::getVolTraded() const {
  return vol_traded;
}

long int Stock::getNumStocks() const{
  return num_stocks;
}

//Setter Methods
void Stock::setCurrPrice(double curr_price){
    this->curr_price = curr_price;
}

void Stock::setVolTraded(long int vol_traded){
  this->vol_traded = vol_traded;
}

void Stock::setNumStocks(long int num_stocks){
  this->num_stocks = num_stocks;
}

long long Stock::getCurrTime() const{
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch() //change this to time since exchange started once the exchange is implemented
    ).count(); 
}

StockFrame Stock::toStockFrame(void) const{
   return StockFrame{
    ticker,
    industry->getName(),
    curr_price,
    vol_traded,
    num_stocks,
    getCurrTime()
  };
}


