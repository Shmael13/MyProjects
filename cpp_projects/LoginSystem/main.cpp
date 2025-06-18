#include <iostream>
#include <limits>
#include "User.h"

int main(){
  User user;
  int choice;

  do {
    std::cout << "\nEnter one of the following numbers. \n1 - Register\n2 - Login\n3 - Exit\n Choice: ";
    std::cin >> choice;

    if (std::cin.fail()){
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      choice = 0;
      continue;

    }

    switch (choice) {
      case 1:
        user.registerUser();
        break;
      case 2:
        user.loginUser();
        break;
      case 3:
        std::cout << "Exiting...\n";
        break;
      default:
        std::cout << "Invalid choice, pick a number between 1 and 3\n";
        break;

    }
  }while(choice != 3);
  return 0;
}

