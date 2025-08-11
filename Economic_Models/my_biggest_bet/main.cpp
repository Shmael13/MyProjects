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
  Stock appl_stock("Apple", techIndustry, 100, 1900, exchange);
  std::vector<Stock*> stock_vector = {&appl_stock};

  //StockFrame sf {appl_stock.toStockFrame()};

  // std::cout << sf.curr_price << "\n";
  appl_stock.setCurrPrice(500);
  // sf = appl_stock.toStockFrame(); 
  // std::cout << sf.curr_price << "\n";
  // std::cout << appl_stock;
  // std::cout << ms_stock;

  // Create and register stocks
  for (Stock* stock: stock_vector){
    exchange.addStock(stock);
  }
  
  PyBinder::initialize_python();
  // Create traders
  std::vector<Trader*> traders = {
      new Trader("smart_trader_1         ", Trade_Algos::smart_trader,         100),
      new Trader("smart_trader_2         ", Trade_Algos::smart_trader,         100),
      new Trader("smart_trader_3         ", Trade_Algos::smart_trader,         100),
      new Trader("smart_trader_4         ", Trade_Algos::smart_trader,         100),
      new Trader("smart_trader_5         ", Trade_Algos::smart_trader,         100),
      new Trader("smart_trader_6         ", Trade_Algos::smart_trader,         100),
      new Trader("smart_trader_7         ", Trade_Algos::smart_trader,         100),
      new Trader("smart_trader_8         ", Trade_Algos::smart_trader,         100),
      new Trader("smart_trader_9         ", Trade_Algos::smart_trader,         100),
      new Trader("smart_trader_10        ", Trade_Algos::smart_trader,         100),

      new Trader("random_trader_1        ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_2        ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_3        ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_4        ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_5        ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_6        ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_7        ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_8        ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_9        ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_10       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_11       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_12       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_13       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_14       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_15       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_16       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_17       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_18       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_19       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_21       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_22       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_23       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_24       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_25       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_26       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_27       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_28       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_29       ", Trade_Algos::random_trader,        100),
      new Trader("random_trader_30       ", Trade_Algos::random_trader,        100),

      new Trader("limit_buy_trader_1     ", Trade_Algos::limit_buy_trader,     100),
      new Trader("limit_buy_trader_2     ", Trade_Algos::limit_buy_trader,     100),
      new Trader("limit_buy_trader_3     ", Trade_Algos::limit_buy_trader,     100),
      new Trader("limit_buy_trader_4     ", Trade_Algos::limit_buy_trader,     100),
      new Trader("limit_buy_trader_5     ", Trade_Algos::limit_buy_trader,     100),
      new Trader("limit_buy_trader_6     ", Trade_Algos::limit_buy_trader,     100),
      new Trader("limit_buy_trader_7     ", Trade_Algos::limit_buy_trader,     100),
      new Trader("limit_buy_trader_8     ", Trade_Algos::limit_buy_trader,     100),
      new Trader("limit_buy_trader_9     ", Trade_Algos::limit_buy_trader,     100),
      new Trader("limit_buy_trader_10    ", Trade_Algos::limit_buy_trader,     100),

      new Trader("market buy trader_1    ", Trade_Algos::market_buy_trader,    100),
      new Trader("market buy trader_2    ", Trade_Algos::market_buy_trader,    100),
      new Trader("market buy trader_3    ", Trade_Algos::market_buy_trader,    100),
      new Trader("market buy trader_4    ", Trade_Algos::market_buy_trader,    100),
      new Trader("market buy trader_5    ", Trade_Algos::market_buy_trader,    100),
      new Trader("market buy trader_6    ", Trade_Algos::market_buy_trader,    100),
      new Trader("market buy trader_7    ", Trade_Algos::market_buy_trader,    100),
      new Trader("market buy trader_8    ", Trade_Algos::market_buy_trader,    100),
      new Trader("market buy trader_9    ", Trade_Algos::market_buy_trader,    100),
      new Trader("market buy trader_10   ", Trade_Algos::market_buy_trader,    100),

      new Trader("Py_Random_1            ", PyBinder::get_py_trade_func("strategies", "random_py_trader"), 100),
      new Trader("Py_Random_2            ", PyBinder::get_py_trade_func("strategies", "random_py_trader"), 100),
      new Trader("Py_Random_3            ", PyBinder::get_py_trade_func("strategies", "random_py_trader"), 100),
      new Trader("Py_Random_4            ", PyBinder::get_py_trade_func("strategies", "random_py_trader"), 100),
      new Trader("Py_Random_5            ", PyBinder::get_py_trade_func("strategies", "random_py_trader"), 100),

      new Trader("Py_MovingAvg_1         ", PyBinder::get_py_trade_func("strategies", "moving_avg_trader"), 100),
      new Trader("Py_MovingAvg_2         ", PyBinder::get_py_trade_func("strategies", "moving_avg_trader"), 100),
      new Trader("Py_MovingAvg_3         ", PyBinder::get_py_trade_func("strategies", "moving_avg_trader"), 100),
      new Trader("Py_MovingAvg_4         ", PyBinder::get_py_trade_func("strategies", "moving_avg_trader"), 100),
      new Trader("Py_MovingAvg_5         ", PyBinder::get_py_trade_func("strategies", "moving_avg_trader"), 100),

      new Trader("LiveNN_trader_1        ", PyBinder::get_py_trade_func("strategies", "get_live_nn_trade_message"), 100), 
  };                         

  std::cout << "INITIAL STATE: \n";
  for (const auto& trader: traders){
    double total_money = trader->getCashBalance();
    printf("%s \t %.3f cash", trader->getTraderId().c_str(), trader->getCashBalance());
    auto& stock_holdings = trader->getStockHoldings();
    
    for (const auto& [ticker, quantity]: stock_holdings){
      total_money += exchange.getStockPrice(ticker) * quantity;
      printf("\t %d %s stocks \t", quantity, ticker.c_str());
    }
    printf(" %.3f TOTAL \n", total_money);

  }    


  // Distribute the stocks we created amongst the traders
  for (Stock* stock: stock_vector){
    int num_traded = static_cast<int>(stock->getNumStocks());
    [[maybe_unused]] int num_tickers = static_cast<int>(stock_vector.size());
    int num_traders = static_cast<int>(traders.size());
    int num_distributed = num_traded / num_traders;
    [[maybe_unused]] int remaining = num_traded % num_traders; // remainder when we distribute to everything

    std::cout << "NUMBER TRADED::" << num_traded << "\n";
    std::cout << "NUM_TRADERS::" << num_traders << "\n";
    std::cout << "NUMBER DISTRIBUTED::::"<< num_distributed << "\n";
    std::cout << "NUM_REMAINING::" << remaining << "\n";
    
    std::string ticker = static_cast<std::string>(stock->getTicker());

    for (auto& trader: traders){
      trader->updateStockHoldings(ticker, num_distributed);
    }
    traders[0]->updateStockHoldings(ticker, 0); //change 0 to remaining if you want to give it to the first trader
  }

  // Main simulation loop    
  for (int tick = 0; tick < 1000; ++tick) {
      exchange.simulateTick(traders);
      //std::this_thread::sleep_for(std::chrono::microseconds(1));
  }
  PyBinder::cleanup_python();


  exchange.printLatestMarketDataframe();
  std::cout << traders[0]->getTicksHeld() << "\n";
 
  auto& winning_trader = traders[0];
  double winner_amount = 0;

  for (const auto& trader: traders){
    double total_money = trader->getCashBalance();
    printf("%s \t %.3f cash", trader->getTraderId().c_str(), trader->getCashBalance());
    auto& stock_holdings = trader->getStockHoldings();
    
    for (const auto& [ticker, quantity]: stock_holdings){
      total_money += exchange.getStockPrice(ticker) * quantity;
      printf("\t %d %s stocks \t", quantity, ticker.c_str());
    }
    printf(" %.3f TOTAL \n", total_money);

    if (winner_amount < total_money){
      winner_amount = total_money;
      winning_trader = trader;
    }
  }    
  std::cout << "\n" << winning_trader->getTraderId() <<"WINS!!! he has: " << winner_amount << "\n";
  exchange.printMarketData();
}
