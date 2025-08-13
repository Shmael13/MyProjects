#pragma once
#include <memory>
#include <queue>
#include <string>
#include "exchange.h"
#include "stock_frame.h"

class Industry;

class Stock {
  struct OrderCompare {
    bool operator()(const Trader_X_Msg &txm1, const Trader_X_Msg &txm2) const {
      const Trades::Trade_Message &t1 = txm1.msg;
      const Trades::Trade_Message &t2 = txm2.msg;

      if (t1.trade_type == Trades::MARKET_BUY ||
          t1.trade_type == Trades::LIMIT_BUY) {
        return t1.price < t2.price; // max-heap for buys
      } else {
        return t1.price > t2.price; // min-heap for sells
      }
    }
  };

private:
  std::string_view ticker;
  std::shared_ptr<Industry> industry;
  double curr_price;
  long int vol_traded;
  long int num_stocks;
  long long getCurrTime() const;
  long long exchange_start_time;
  std::priority_queue<Trader_X_Msg, std::vector<Trader_X_Msg>, OrderCompare>
      buy_book;
  std::priority_queue<Trader_X_Msg, std::vector<Trader_X_Msg>, OrderCompare>
      sell_book;

public:
  // Init
  Stock(std::string_view ticker, std::shared_ptr<Industry> industryPtr,
        double curr_price, long int num_stocks, Exchange &exchange);

  // Attribute Getter Methods
  std::string_view getIndustryName() const;
  std::shared_ptr<Industry> getIndustry() const;
  std::string_view getTicker() const;
  double getCurrPrice() const;
  long int getVolTraded() const;
  long int getNumStocks() const;

  // Attribute Setter Methods
  void setCurrPrice(double curr_price);
  void setVolTraded(long int vol_traded);
  void setNumStocks(long int num_stocks);

  // Snapshot Getter Method
  StockFrame toStockFrame(void) const;

  auto &getSellBook() { return sell_book; }

  void pushOrder(Trader_X_Msg txm);

  auto &getBuyBook() { return buy_book; }
};

constexpr std::ostream &operator<<(std::ostream &out, Stock stock) {
  out << "\n"
      << stock.getTicker() << "\n"
      << "\tIndustry:      " << stock.getIndustryName() << "\n"
      << "\tPrice:    " << stock.getCurrPrice() << "\n"
      << "\tVolume Traded: " << stock.getVolTraded() << "\n"
      << "\tNumber Stocks: " << stock.getNumStocks() << "\n";
  return out;
}
