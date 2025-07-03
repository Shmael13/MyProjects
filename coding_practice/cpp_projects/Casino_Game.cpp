#include <iostream>
#include <string>
#include <stdio.h>

long long deposit_amount;

int rand_num(){
  return rand()%11;
}

int main(){
  srand(time(NULL));
  std::cout << "Enter the $$ Deposit amount ";
  std::cin >> deposit_amount;
  int i; 
  while (true) {
    
    if (i = rand_num()){
      std::cout << "Exit! " << i << "\n";  
    }
    else{
      break;
    }
  }
}

