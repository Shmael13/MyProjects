#include <iostream>
#include <memory>
#include <thread>
#include "stock.h"
#include "exchange.h"
#include "Random.h"
#include "utils/industry.h"
#include "utils/trade_message.h"
#include "trader_algos/trader_algos.h"

int main(){
  
  Exchange exchange;
  auto techIndustry = std::make_shared<Industry>("techIndustry");

  Stock ms_stock("MSFT", techIndustry, 2000, 1000000, exchange);
  Stock appl_stock("Apple", techIndustry, 1900, 100, exchange);
  std::vector<Stock*> stock_vector = {&appl_stock};

  StockFrame sf {appl_stock.toStockFrame()};

  std::cout << sf.curr_price << "\n";
  appl_stock.setCurrPrice(500);
  sf = appl_stock.toStockFrame(); 
  std::cout << sf.curr_price << "\n";
  std::cout << appl_stock;
  std::cout << ms_stock;
  

  // Create and register stocks
  for (Stock* stock: stock_vector){
    exchange.addStock(stock);
  }
  

  // Create traders
  std::vector<Trader*> traders = {
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("random_trader", Trade_Algos::random_trader, 100),
      new Trader("smart trader", Trade_Algos::smart_trader, 100),
      new Trader("smart trader", Trade_Algos::smart_trader, 100),
      new Trader("Python trader", Trade_Algos::py_trader, 100),
  };                         

  // Distribute the stocks we created amongst the traders
  for (Stock* stock: stock_vector){
    int num_traded = static_cast<int>(stock->getNumStocks());
    int num_tickers = static_cast<int>(stock_vector.size());
    
    int num_distributed = num_traded / num_tickers;
    [[maybe_unused]] int remaining = num_traded % num_tickers; // remainder when we distribute to everything
    
    std::string ticker = static_cast<std::string>(stock->getTicker());

    for (auto& trader: traders){
      trader->updateStockHoldings(ticker, num_distributed);
    }
    traders[0]->updateStockHoldings(ticker, 0); //change 0 to remaining if you want to give it to the first trader
  }

  // Main simulation loop    
  for (int tick = 0; tick < 500; ++tick) {
      exchange.simulateTick(traders);
      //std::this_thread::sleep_for(std::chrono::microseconds(1));
  }
  exchange.printMarketData();
  std::cout << traders[0]->getTicksHeld() << "\n";
  
  for (const auto& trader: traders){
    double total_money = trader->getCashBalance();
    printf("%s has %.3f cash", trader->getTraderId().c_str(), trader->getCashBalance());
    auto& stock_holdings = trader->getStockHoldings();
    
    for (const auto& [ticker, quantity]: stock_holdings){
      total_money += exchange.getStockPrice(ticker) * quantity;
      printf("\t %d %s stocks \t", quantity, ticker.c_str());
    }
    printf("%.3f TOTAL\n", total_money);
  }    
  exchange.printLatestMarketDataframe();
  std::cout << appl_stock;

  

  

}
