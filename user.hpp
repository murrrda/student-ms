#ifndef USER_HPP
#define USER_HPP


#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class User {
private:
    std::string username;
    std::string password;
public:
    User():username(""), password("") {}
    User(const std::string& username, const std::string& password):username(username), password(password) {}
    
    std::string getUsername() const {
        return username;
    }
    std::string getPassword() const {
        return password;
    }
    void setUsername(const std::string& username) {
        this->username = username;
    }
    void setPassword(const std::string& password) {
        this->password = password;
    }
};

#endif // !USER_HPP
