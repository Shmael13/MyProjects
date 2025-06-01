#include <iostream>
#include <memory>
#include <thread>
#include "utils/industry.h"
#include "exchange.h"
#include "stock.h"


int main(){
  Exchange exchange;
  std::cout << Exchange::getExchangeStartTime() << "\n";
  std::shared_ptr<Industry> techIndustry = std::make_shared<Industry>("Technology");

  Stock apple("APPL", techIndustry, 100.5, 1000);
  Stock microsoft("MSFT", techIndustry, 152.5, 100);

  std::vector<Stock*> stocks = techIndustry->getStocks();

  StockFrame stockInfo = stocks[0]->toStockFrame();
  std::cout << "Stocks are: " << stockInfo.ticker << std::endl;
  stockInfo = stocks[1]->toStockFrame();
  std::cout << "Stocks are: " << stockInfo.ticker << std::endl;
  
  std::cout << "Stocks in " << techIndustry->getName() << " industry\n";
  std::this_thread::sleep_for(std::chrono::seconds(2));
  int i = 0;
  while(1){
    std::cout << "l";//tests that the time works
    i++;
    if (i == 10000){std::cout << "\n"; break;}
  }
  
  std::cout << exchange.getTimeSinceExchangeStart() << "\n";
  exchange.updateMarketData("APPL", stockInfo);

  return 0;

}

