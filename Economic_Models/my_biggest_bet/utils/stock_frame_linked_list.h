#pragma once
#include "stock_frame.h"
#include <memory>

struct StockFrameLL{
  StockFrame sf;
  std::unique_ptr<StockFrameLL> next;

  StockFrameLL(const StockFrame &sf_) : sf(sf_), next(nullptr){} 

  // Copy constructor — deep copy
  StockFrameLL(const StockFrameLL& other)
      : sf(other.sf),
        next(other.next ? std::make_unique<StockFrameLL>(*other.next) : nullptr) {}

  // Copy assignment operator — deep copy
  StockFrameLL& operator=(const StockFrameLL& other) {
      if (this != &other) {
          sf = other.sf;
          next = other.next ? std::make_unique<StockFrameLL>(*other.next) : nullptr;
      }
      return *this;
  }
  StockFrameLL(StockFrameLL &&other)noexcept
    :sf(std::move(other.sf)), next(std::move(other.next)){}
  
  StockFrameLL& operator= (StockFrameLL&& other) noexcept{
    if (this != &other){
      sf = std::move(other.sf);
      next = std::move(other.next);
    }
    return *this;
  }

};

//StockFrameLL* create_sfll(StockFrame sf){
//  StockFrameLL* new_node = new StockFrameLL();
//  new_node->sf = sf;
//  new_node->next = nullptr;
//  return new_node;
//}

inline std::unique_ptr<StockFrameLL> insert_node(StockFrame sf, std::unique_ptr<StockFrameLL> head){
  //Assumes linked list is not empty
  std::unique_ptr<StockFrameLL> new_node = std::make_unique<StockFrameLL>(sf);
  new_node->next = std::move(head);
  return new_node;
}

inline int del_sfll(std::unique_ptr<StockFrameLL> &head){
  head.reset();
  return 0;
}
