#include <iostream>
#include <memory>
#include "stock.h"
#include "industry.h"


int main(){
  std::shared_ptr<Industry> techIndustry = std::make_shared<Industry>("Technology");

  Stock apple("APPL", 100.5, techIndustry);
  Stock microsoft("MSFT", 152.5, techIndustry);

  std::vector<Stock*> stocks = techIndustry->getStocks();
  std::cout << "Stocks in " << techIndustry->getName() << " industry\n";
  for (Stock* stock : stocks){
    std::cout << "Inside for loop";
    std::cout << "- " << stock->getTicker() << " ($" << stock->getCurrPrice() << ")\n";
  }

  return 0;

}

