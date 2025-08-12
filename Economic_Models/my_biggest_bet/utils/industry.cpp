#include "industry.h"
#include "../stock.h"

Industry::Industry(const std::string_view industry_name, double avg_growth)
    : industry_name(industry_name), avg_growth(avg_growth), stocks() {}

void Industry::addStock(Stock *stock) { stocks.push_back(stock); }

std::string_view Industry::getName() const { return industry_name; }

std::vector<Stock *> Industry::getStocks() const { return stocks; }

double Industry::getAvgGrowth() const { return avg_growth; }

void Industry::setAvgGrowth(double avg_growth) {
  this->avg_growth = avg_growth;
}
