#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <limits>

int main()
{
  const double CONVERSION = 1.609344;

  std::cout << "Please type in the distance in miles\n>>>";
  double miles = 0;
  std::cin >>  miles;

  //input error handling
  while(!std::cin.good()){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Please give a number\n";
    std::cin >> miles;
  }

  std::cout << "The distance of miles in km is: " << miles * CONVERSION << std::endl;
}
