#include <string>
#include <iostream>

int main(){
  std::string name;
  int age;

  std::cout << "Enter your full name: ";
  std::getline(std::cin >> std::ws,  name);
  std::cout << "Enter your age: ";
  std::cin >>age;
  
  std::cout << "Your age + length of name is: " << age + std::ssize(name);
  std::cout << name;
}
