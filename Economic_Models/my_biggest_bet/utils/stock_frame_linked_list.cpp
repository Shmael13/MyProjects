#pragma once
#include "stock_frame.h"

struct StockFrameLL{
  StockFrame sf;
  StockFrameLL* next;

};

StockFrameLL* create_sfll(StockFrame sf){
  StockFrameLL* new_node = new StockFrameLL();
  new_node->sf = sf;
  new_node->next = nullptr;
  return new_node;
}

StockFrameLL* insert_node(StockFrame sf, StockFrameLL* head){
  //Assumes linked list is not empty
  Node* new_node = new Node();
  new_node->sf = sf;
  new_node->next = head;
  return new_node;
}

int del_sfll(StockFrameLL* head){
  if (head == nullptr){return -1;}
  StockFrameLL* temp;
  while (head->next != nullptr){
    temp = head->next;
    delete head;
    head = temp;
  }
  delete head;
  return 0;
}
