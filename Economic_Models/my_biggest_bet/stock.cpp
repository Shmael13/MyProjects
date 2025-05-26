#include "stock.h"
#include "industry.h"

Stock::Stock(std::string ticker, double curr_price, std::shared_ptr<Industry> industryPtr)
    :industry(industryPtr), ticker(ticker), curr_price(curr_price){
    industry->addStock(this);
}

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

void Stock::setCurrPrice(double curr_price){
    this->curr_price = curr_price;
}

