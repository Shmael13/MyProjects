//User.h

#pragma once
#include <string>

class User{
  private:
    std::string username;
    std::string password;
public:
    void registerUser();
    bool loginUser();
    
    std::string encryptDecrypt(std::string_view input);
};
