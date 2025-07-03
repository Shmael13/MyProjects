#include "User.h"
#include <iostream>
#include <fstream>

std::string User::encryptDecrypt(std::string_view input){
  std::string output = static_cast<std::string>(input);
  char key = 'K'; //simple XOR encryption
  for (char &c : output){
    c ^= key;
  }
  return output;
}

void User::registerUser(){
  std::cout << "Enter Username: ";
  std::getline(std::cin >> std::ws, username);
  std::cout << "Enter Password: ";
  std::getline(std::cin, password);
  
  std::ofstream file("users.txt", std::ios::app);
  file << username << " " <<encryptDecrypt(password) << "\n";
  file.close();

  std::cout << "Successfully registered user\n";
}

bool User::loginUser(){
  std::cout << "Enter Username: ";
  std::getline(std::cin >> std::ws, username);
  std::cout << "Enter Password: ";
  std::getline(std::cin, password);
  
  std::ifstream file("users.txt");
  std::string user, pass;
  while (file >> user >> pass){
    if (user == username && pass == encryptDecrypt(password)){
      std::cout << "Logged in Successfully!\n";
      return true;
    }
  }
  std::cout << "Invalid username or password\n";
  return false;
}
