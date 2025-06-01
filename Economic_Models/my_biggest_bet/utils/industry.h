#pragma once
#include <string>
#include <vector>

class Stock;
class Industry{
  private:  
    std::string industry_name;
    double avg_growth;
    std::vector<Stock*> stocks;
  
  public:
    Industry(const std::string& industry_name, double avg_growth=1.0);
    void addStock(Stock* stock);
    std::string getName() const;
    std::vector<Stock*> getStocks() const;
    double getAvgGrowth() const;
    void setAvgGrowth(double avg_growth);
};
