#include <iostream>
#include <memory>
#include "stock.h"
#include "utils/industry.h"


int main(){
  std::shared_ptr<Industry> techIndustry = std::make_shared<Industry>("Technology");

  Stock apple("APPL", techIndustry, 100.5, 1000);
  Stock microsoft("MSFT", techIndustry, 152.5, 100);

  std::vector<Stock*> stocks = techIndustry->getStocks();

  StockFrame stockInfo = stocks[0]->toStockFrame();
  std::cout << "Stocks are: " << stockInfo.ticker << std::endl;
  stockInfo = stocks[1]->toStockFrame();
  std::cout << "Stocks are: " << stockInfo.ticker << std::endl;
  
  std::cout << "Stocks in " << techIndustry->getName() << " industry\n";

  return 0;

}

