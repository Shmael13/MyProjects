#pragma once
#include <string>
#include <memory>

class Industry;

class Stock{
  private:
    std::shared_ptr<Industry> industry;
    std::string ticker;
    double curr_price;
public:
  Stock(std::string ticker, double curr_price, std::shared_ptr<Industry> industryPtr);
  std::string getIndustryName() const;
  std::shared_ptr<Industry> getIndustry() const;
  std::string getTicker() const;
  double getCurrPrice() const;
  void setCurrPrice(double curr_price);
};
