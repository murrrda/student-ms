#include <tuple>
#include <iostream>
#include "user.hpp"
#include "student.hpp"
#include "courses.hpp"
#include "enrollment.hpp"
#include "admin.hpp"
#include "config.h"

int menuAdmin();
int menuStudent();
void insertNewStudent(sql::Connection*, const Student&);
void insertNewCourse(sql::Connection*, const Courses&);
std::tuple<int, int> authenticate(sql::Connection*, const User&);
void createUserStudent(sql::Connection*, const User&, const Student&); 
void createUserAdmin(sql::Connection*, const User&, const Admin&);
bool checkIfAvailableUsername(sql::Connection*, const std::string&);

int main() {	
    sql::Driver *driver;
    sql::Connection *con;
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", USERNAME, PASSWORD);
    con->setSchema("studentMS");

    std::string username = "";
    std::string password = "";
    std::string firstName = "";
    std::string lastName = "";
    std::string index = "";
    int age = 0;
    User usr;
    Student student;
    Admin admin;
    Courses course;
    std::tuple<int, int> x;
    x = std::make_tuple(0, 0);
    int prompt = 0;
    int registerType = 0;
    int newId = 0;
    std::string courseName = "";
    int prompt2 = 0;
    std::string newInstructor = "";
    do{
        std::cout << "1. Login" << '\n';
        std::cout << "2. Register" << '\n';
        std::cin >> prompt;
        switch (prompt) {
            case 1: 
                do {
                    std::cout << "username: ";
                    std::cin >> username;
                    std::cout << "password: ";
                    std::cin >> password;
                    usr.setUsername(username);
                    usr.setPassword(password);
                    x = authenticate(con, usr);
                    
                    std::cout << std::get<0>(x) << " " << std::get<1>(x);
                } while(x == std::make_tuple(2, 0));
                break;
            case 2: 
                do {
                    std::cout << "Register as:\n1. Student\n2. Admin\n";
                    std::cin >> registerType;
                    switch (registerType) {
                        case 1:
                            std::cout << "username: ";
                            std::cin >> username;
                            if(checkIfAvailableUsername(con, username)) {
                                std::cout << "password: ";
                                std::cin >> password;
                                std::cout << "First name: ";
                                std::cin >> firstName;
                                std::cout << "Last name: ";
                                std::cin >> lastName;
                                std::cout << "Index: ";
                                std::cin >> index;
                                std::cout << "Age: ";
                                std::cin >> age; 
                               
                                student.set_age(age);
                                student.set_index(index);
                                student.set_last_name(lastName);
                                student.set_first_name(firstName);

                                usr.setUsername(username);
                                usr.setPassword(password);
                                createUserStudent(con, usr, student); //inserts new data in sql tables

                                x = authenticate(con, usr);
                            }
                            else {
                                std::cout << "Username already taken!\n";
                                registerType = 0;
                            }
                            break;
                        case 2:
                            std::cout << "username: ";
                            std::cin >> username;
                            if(checkIfAvailableUsername(con, username)) {
                                std::cout << "password: ";
                                std::cin >> password;
                                std::cout << "First name: ";
                                std::cin >> firstName;
                                std::cout << "Last name: ";
                                std::cin >> lastName;

                                usr.setUsername(username);
                                usr.setPassword(password);

                                admin.setFirstName(firstName);
                                admin.setLastName(lastName);

                                createUserAdmin(con, usr, admin); //inserts new data in sql tables

                                x = authenticate(con, usr);
                            }
                            else {
                                std::cout << "Username already taken!\n";
                                registerType = 0;
                            }
                            break;
                    }
                }while(registerType < 1 || registerType > 2);
                break;
            default:
                std::cout << "You've entered the wrong option!\n";
                break;
        }
    }while (prompt < 1 || prompt > 2);   

    if(std::get<0>(x) == 0) { //is student
        sql::PreparedStatement* stmt = con->prepareStatement("SELECT first_name, last_name, student_index, age FROM students  WHERE student_id = ?");
        stmt->setInt(1, std::get<1>(x));

        sql::ResultSet* res = stmt->executeQuery();

        if(res->next()) {
            Student student(usr, res->getString("first_name"), res->getString("last_name"), res->getString("student_index"), res->getInt("age"));
            prompt = 0;
            do{
                prompt = menuStudent();
                switch (prompt) {
                    case 0: 
                        std::cout << "Exiting...\n";
                        break;
                    case 1:
                        student.listCourses(con);
                        break;
                    case 2:
                        student.listStudentsEnrolledWithMe(con);
                        break;                
                    case 3:
                        student.enrollInCourse(con);
                        break;
                    default:
                        std::cout << "You chose the wrong option!\n";
                        break;
                }
            }while(prompt != 0);
        }
    }   

    else {              //is admin
        sql::PreparedStatement* stmt = con->prepareStatement("SELECT first_name, last_name FROM admins WHERE admin_id = ?");
        stmt->setInt(1, std::get<1>(x));
        sql::ResultSet* res = stmt->executeQuery();

        if(res->next()) {
            Admin admin(res->getString("first_name"), res->getString("last_name"));
        }
        prompt = 0;
        do{
            prompt = menuAdmin();
            switch (prompt) {
                case 0: 
                    std::cout << "Exiting...\n";
                    break;
                case 1:
                    admin.listStudents(con);
                    break;
                case 2:
                    admin.listCourses(con);
                    break;                
                case 3:
                    do{
                        prompt2 = admin.updateStudentInfoMenu();
                        switch (prompt2) {
                            case 0:
                                std::cout << "Done changing\n";
                                break;
                            case 1:
                                std::cout << "Student's ID: ";
                                std::cin >> newId;
                                std::cout << "New name: ";
                                std::cin >> firstName;
                                admin.updateStudentFirstName(con, newId, firstName);
                                break;
                            case 2:
                                std::cout << "Student's ID: ";
                                std::cin >> newId;
                                std::cout << "New name: ";
                                std::cin >> lastName;
                                admin.updateStudentLastName(con, newId, lastName);
                                break;
                            case 3:
                                std::cout << "Student's ID: ";
                                std::cin >> newId;
                                std::cout << "New index: ";
                                std::cin >> index;
                                admin.updateStudentIndex(con, newId, index);
                                break;
                            case 4:
                                std::cout << "Student's ID: ";
                                std::cin >> newId;
                                std::cout << "New age: ";
                                std::cin >> age;
                                admin.updateStudentByAge(con, newId, age);
                                break;
                            default:
                                std::cout << "wrong choice!\n";
                                break;
                        }
                    }while(prompt2 != 0);
                    break;
                case 4:
                    admin.listEnrollments(con);
                    break;
                case 5:
                    std::cout << "Choose one option: \n1. By index\n2. By ID";
                    switch (prompt2) {
                        case 1:
                            std::cin >> index;
                            admin.deleteStudent(con, index);
                            break;
                        case 2:
                            std::cin >> newId;
                            admin.deleteStudent(con, newId);
                            break;
                        default:
                            std::cout << "Wrong option\n";
                    }
                    break;
                case 6:
                    std::cout << "Enter course name\n";
                    std::cin >> courseName;
                    admin.deleteCourse(con, courseName);
                    break;
                case 7:
                    std::cout << "Course name: ";
                    std::cin >> courseName;
                    std::cout << "Instructor name: ";
                    std::cin >> newInstructor;

                    course.set_name(courseName);
                    course.set_instructor(newInstructor);

                    admin.insertNewCourse(con, course);

                    break;
                default:
                    std::cout << "You chose the wrong option!\n";
                    break;
            }
        }while(prompt != 0);
    }
    delete con;
    return 0;
}

int menuStudent() {
    std::cout << "\n\t Choose one option:\n";
    std::cout << "\t\t0. Exit program\n";
    std::cout << "\t\t1. List all courses\n";
    std::cout << "\t\t2. List students enrolled on your courses\n";
    std::cout << "\t\t3. Enroll in course \n";
    int opcija;
    std::cin >> opcija;

    return opcija;
}
std::tuple<int, int> authenticate(sql::Connection* con, const User& usr) {
    sql::PreparedStatement* auth = con->prepareStatement("SELECT * FROM users WHERE username = ? AND password = ?");
    auth->setString(1, usr.getUsername());
    auth->setString(2, usr.getPassword());
    sql::ResultSet* res =  auth->executeQuery();

    int resInt = 0;
    if(res->next()) {
        if(res->getInt("admin_id") == 0) {
            resInt = res->getInt("student_id");
            delete auth;
            delete res;
            return std::make_tuple(0, resInt);
        }
        else {
            resInt = res->getInt("admin_id");
            delete auth;
            delete res;
            return std::make_tuple(1, resInt); 
        }
    }
    delete auth;
    delete res;
    return std::make_tuple(2, 0);
}
int menuAdmin() {
    std::cout << "\n\t Choose one option:\n";
    std::cout << "\t\t0. Exit program\n";
    std::cout << "\t\t1. List all students\n";
    std::cout << "\t\t2. List all courses\n";
    std::cout << "\t\t3. Update student info menu\n";
    std::cout << "\t\t4. List all enrollments\n";
    std::cout << "\t\t5. Delete student\n";
    std::cout << "\t\t6. Delete course\n";
    std::cout << "\t\t7. Insert new course\n";
    int opcija;
    std::cin >> opcija;

    return opcija;
}
void createUserStudent(sql::Connection* con, const User& usr, const Student& student) {
    std::cout << "\n\n\n" << student.get_first_name();
    //insert student
    sql::PreparedStatement* stmt = con->prepareStatement("INSERT INTO students (first_name, last_name, student_index, age) VALUES (?, ?, ?, ?)");
    stmt->setString(1, student.get_first_name());
    stmt->setString(2, student.get_last_name());
    stmt->setString(3, student.get_index());
    stmt->setInt(4, student.get_age());
    stmt->executeUpdate();
    delete stmt;

    //get new student id
    int student_id = 0;
    sql::PreparedStatement* getId = con->prepareStatement("SELECT student_id FROM students where student_index = ?");
    getId->setString(1, student.get_index());
    sql::ResultSet* res = stmt->executeQuery();

    if(res->next())
        student_id = res->getInt("student_id");

    delete res;
    delete getId;

    //insert user
    sql::PreparedStatement* stmt2 = con->prepareStatement("INSERT INTO users (username, password, student_id) VALUES (?, ?, ?)");
    stmt2->setString(1, usr.getUsername());
    stmt2->setString(2, usr.getPassword());
    stmt2->setInt(3, student_id);
    stmt2->executeUpdate();

    delete stmt2;
}
void createUserAdmin(sql::Connection* con, const User& usr, const Admin& admin) {
    //insert admin 
    sql::PreparedStatement* stmt = con->prepareStatement("INSERT INTO admins (first_name, last_name) VALUES (?, ?)");
    stmt->setString(1, admin.getFirstName()); 
    stmt->setString(2, admin.getLastName());
    stmt->executeUpdate();
    delete stmt;

    //get new admin id
    int admin_id = 0;
    sql::PreparedStatement* getId = con->prepareStatement("SELECT admin_id FROM admins where first_name = ? AND last_name = ?");
    getId->setString(1, admin.getFirstName());
    getId->setString(2, admin.getLastName());
    sql::ResultSet* res = stmt->executeQuery();

    if(res->next())
        admin_id = res->getInt("admin_id");

    delete res;
    delete getId;

    //insert user
    sql::PreparedStatement* stmt2 = con->prepareStatement("INSERT INTO users (username, password, admin_id) VALUES (?, ?, ?)");
    stmt2->setString(1, usr.getUsername());
    stmt2->setString(2, usr.getPassword());
    stmt2->setInt(3, admin_id);
    stmt2->executeUpdate();

    delete stmt2;
}
bool checkIfAvailableUsername(sql::Connection* con, const std::string& username) {
    sql::PreparedStatement* stmt = con->prepareStatement("select COUNT(*) as count from users where username = ?");
    stmt->setString(1, username);
    sql::ResultSet* res = stmt->executeQuery();

    int count = 0;
    res->next();
    count = res->getInt("count");

    delete stmt;
    delete res;

    return !(count > 0);
}
