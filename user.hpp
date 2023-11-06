#ifndef USER_HPP
#define USER_HPP

#include <iostream>

class User {
private:
    std::string username;
    std::string password;
public:
    User(const std::string& username, const std::string& password):username(username), password(password) {}
    
    std::string getUsername() const {
        return username;
    }
    std::string getPassword() const {
        return password;
    }

};

#endif // !USER_HPP
