#ifndef STUDENT_HPP
#define STUDENT_HPP

#include "user.hpp"

class Student : public User{
private:
    std::string first_name;
    std::string last_name;
    std::string index;
    int age;
public:
    Student(std::string first_name = "fake first name", std::string last_name = "fake last name", std::string index = "INXX-YYYY", int age = 0): User(), first_name(first_name), last_name(last_name), index(index), age(age) {}
    Student(const User& usr, std::string first_name = "fake first name", std::string last_name = "fake last name", std::string index = "INXX-YYYY", int age = 0): User(usr), first_name(first_name), last_name(last_name), index(index), age(age) {}

    std::string get_first_name() const{
            return first_name;
    }
    
    std::string get_last_name() const{
            return last_name;
    }
    
    std::string get_index() const {
            return index;
    }
    
    int get_age() const {
            return age;
    }
    
    void set_first_name(std::string new_name) {
            first_name = new_name;
    }
    
    void set_last_name(const std::string& new_name) {
            last_name = new_name;
    }	
    
    void set_index(std::string new_index) {
            index = new_index;
    }
    
    void set_age(int new_age) {
            age = new_age;
    }
    
    void print_info() const{
            std::cout << '\n';
            std::cout << "Student name: " << first_name << ' ' << last_name << '\n';
            std::cout << "Index: " << index << '\n';
            std::cout << "Age: " << age << '\n' << '\n';
    }
    void listCourses(sql::Connection* con) const {
        sql::Statement *stmt;
        sql::ResultSet *res;
        stmt = con->createStatement();
        int i = 0;
        std::cout << '\n';
        res = stmt->executeQuery("select course_name, instructor from courses");
        while(res->next()) {
            i++;
            std::cout << i << "." << '\t'  << "Course name: " << res->getString("course_name");
            std::cout << ", Instructor: " << res->getString("instructor");
            std::cout << '\n';
        }
        delete res;
        delete stmt;
    }
    int getStudentIdByIndex(sql::Connection* con) {
        sql::PreparedStatement* stmt = con->prepareStatement("SELECT student_id FROM students WHERE student_index = ?");
        stmt->setString(1, index);
        sql::ResultSet* res = stmt->executeQuery();

        int student_id = 0;

        if(res->next()) {
                student_id = res->getInt("student_id");
        }

        delete stmt;
        delete res;
        
        return student_id;
    }
    void listMyCourses(sql::Connection* con) {
        int student_id = getStudentIdByIndex(con);
        sql::PreparedStatement* stmt = con->prepareStatement("SELECT * FROM enrollment WHERE student_id = ?");
        stmt->setInt(1, student_id);
        sql::ResultSet* res = stmt->executeQuery();

        int i = 0;
        while(res->next()) {
            sql::PreparedStatement* stmt2 = con->prepareStatement("select course_name, instructor from courses where course_id = ?");
            stmt2->setInt(1, res->getInt("course_id"));
            sql::ResultSet* res2 = stmt2->executeQuery();
            i++;
            if(res2->next()) {
                std::cout << i << "." << '\t'  << "Course name: " << res2->getString("course_name");
                std::cout << ", Instructor: " << res2->getString("instructor");
                std::cout << '\n';
                delete res2;
                delete stmt2;
            }
        }

        delete res;
        delete stmt;
    }
    void listStudentsEnrolledWithMe(sql::Connection* con) {
         int student_id = getStudentIdByIndex(con);
        sql::PreparedStatement* stmt = con->prepareStatement(
            "SELECT s.first_name, s.last_name, c.course_name "
            "FROM enrollment e "
            "JOIN courses c ON e.course_id = c.course_id "
            "JOIN students s ON e.student_id = s.student_id "
            "WHERE c.course_id IN (SELECT course_id FROM enrollment WHERE student_id = ?) "
            "AND e.student_id != ?"
        );
        stmt->setInt(1, student_id);
        stmt->setInt(2, student_id);
        sql::ResultSet* res = stmt->executeQuery();

        while (res->next()) {
            std::cout << "Student koji slusaju kurs \"" << res->getString("course_name") << "\" sa tobom su: ";
            std::cout << res->getString("first_name") << ' ' << res->getString("last_name") << std::endl;
        }

        delete stmt;
        delete res;
    }
    void enrollInCourse(sql::Connection* con) {
        int student_id = getStudentIdByIndex(con);
        //show list of courses
	sql::Statement *stmt;
	sql::ResultSet *res;
	stmt = con->createStatement();
	std::cout << '\n';
	res = stmt->executeQuery("select course_id, course_name, instructor from courses");
	while(res->next()) {
		std::cout << res->getInt("course_id") << "." << ' '  << "Course name: " << res->getString("course_name");
		std::cout << ", Instructor: " << res->getString("instructor");
		std::cout << '\n';
	}
	
	delete res;
	delete stmt;

        int course_id = 0;
        std::cout << "Enter course id: ";
        std::cin >> course_id;
        
        sql::PreparedStatement *stmt2;
        sql::ResultSet *res2;

        stmt2 = con->prepareStatement("SELECT course_id FROM courses WHERE course_id = ?");
        stmt2->setInt(1, course_id);
        res2 = stmt2->executeQuery();

        if(res2->next()) {
            sql::PreparedStatement* stmt3 = con->prepareStatement("INSERT INTO enrollment(student_id, course_id) VALUES (?, ?)");
            stmt3->setInt(1, student_id);
            stmt3->setInt(2, course_id);
            stmt3->executeUpdate();
            delete stmt3;
        }

        delete res2;
        delete stmt2;
    }
};

#endif
