#include <iostream>



int readNumber(){
  int x{};
  std::cout << "Input an integer ";
  std::cin >> x;
  return x;
}

void writeAnswer(int x){
  std::cout << "The answer is: " << x << "\n";
}

int main (){
  int x{};
  x = readNumber();
  writeAnswer(x);
  return 0;
}
