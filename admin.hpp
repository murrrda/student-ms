#ifndef ADMIN_HPP
#define ADMIN_HPP 

#include "user.hpp"

class Admin : public User {
private:
    std::string first_name;
    std::string last_name;
public:
    Admin(const std::string& first_name = "fake first", const std::string& last_name = "fake last"):first_name(first_name), last_name(last_name){}

    std::string getFirstName() const {
        return first_name;
    }
    std::string getLastName() const {
        return last_name;
    }
    void setFirstName(const std::string& first_name) {
        this->first_name = first_name;
    }
    void setLastName(const std::string& last_name) {
        this->last_name = last_name;
    }
    void updateMenu(sql::Connection* con) {

    }
    int updateStudentInfoMenu() {
        int choice;
        std::cout << "\n\t Choose one option:\n";
        std::cout << "\t\t0. Back to menu\n";
        std::cout << "\t\t1. Update First Name\n";
        std::cout << "\t\t2. Update Last Name\n";
        std::cout << "\t\t3. Update Index\n";
        std::cout << "\t\t4. Update age\n";
        std::cin >> choice;
        return choice;
    }
    void updateStudentFirstName(sql::Connection* con, int student_id, const std::string& newFirstName) {
        if(student_id != 0) {
            sql::PreparedStatement* stmtName = con->prepareStatement("UPDATE students SET first_name= ? WHERE student_id = ?");
            stmtName->setString(1, newFirstName);
            stmtName->setInt(2, student_id);
            stmtName->executeUpdate();

            delete stmtName;
        }
            
    }
    void updateStudentLastName(sql::Connection* con, int student_id, const std::string& newLastName) {
        if(student_id != 0) {
            sql::PreparedStatement* stmtName = con->prepareStatement("UPDATE students SET last_name= ? WHERE student_id = ?");
            stmtName->setString(1, newLastName);
            stmtName->setInt(2, student_id);
            stmtName->executeUpdate();

            delete stmtName;
        }
    }
    void updateStudentIndex(sql::Connection* con, int student_id, const std::string& newIndex) {
        if(student_id != 0) {
            sql::PreparedStatement* stmtName = con->prepareStatement("UPDATE students SET student_index= ? WHERE student_id = ?");
                stmtName->setString(1, newIndex);
                stmtName->setInt(2, student_id);
                stmtName->executeUpdate();

                delete stmtName;
            }
    }
    void updateStudentByAge(sql::Connection* con, int student_id, int age) {
	if(student_id != 0) {
            sql::PreparedStatement* stmtName = con->prepareStatement("UPDATE students SET age = ? WHERE student_id = ?");
            stmtName->setInt(1, age);
            stmtName->setInt(2, student_id);
            stmtName->executeUpdate();

            delete stmtName;
	}
}
    void listStudents(sql::Connection* con) {
	sql::Statement *stmt;
	sql::ResultSet *res;
	stmt = con->createStatement();
	int i = 0;	
	std::cout << '\n';
	res = stmt->executeQuery("select student_id, first_name, last_name, student_index, age from students");
	while(res->next()) {
            i++;
            std::cout << res->getInt("student_id") << '\t'  << "First name: " << res->getString("first_name");
            std::cout << ", Second name: " << res->getString("last_name");	
            std::cout << ", Index: " << res->getString("student_index");
            std::cout << ", Age: " << res->getInt("age");
            std::cout << '\n';
	}
	delete res;
	delete stmt;
    }
    void listCourses(sql::Connection* con) {
	sql::Statement *stmt;
	sql::ResultSet *res;
	stmt = con->createStatement();
	int i = 0;
	std::cout << '\n';
	res = stmt->executeQuery("select course_id, course_name, instructor from courses");
	while(res->next()) {
            i++;
            std::cout << res->getInt("course_id") << '\t'  << "Course name: " << res->getString("course_name");
            std::cout << ", Instructor: " << res->getString("instructor");
            std::cout << '\n';
	}
	
	delete res;
	delete stmt;
    }
    void listEnrollments(sql::Connection* con) {
	sql::Statement* stmt;
	sql::ResultSet* res;
	stmt = con->createStatement();
	res = stmt->executeQuery("select student_id, course_id from enrollment");
	
	while(res->next()) {
            sql::PreparedStatement* stmtStudent = con->prepareStatement("select first_name, last_name from students where student_id = ?");
            stmtStudent->setInt(1, res->getInt("student_id"));
            sql::ResultSet* resStudent = stmtStudent->executeQuery();

            if(resStudent->next()) {
            std::cout << resStudent->getString("first_name") << ' ' << resStudent->getString("last_name");
            std::cout << " slusa ";
            }

            delete stmtStudent;
            delete resStudent;
            
            sql::PreparedStatement* stmtCourse = con->prepareStatement("select course_name, instructor from courses where course_id = ?");
            stmtCourse->setInt(1, res->getInt("course_id"));
            sql::ResultSet* resCourse = stmtCourse->executeQuery();
            if(resCourse->next()) {
            std::cout << resCourse->getString("course_name") << " by " << resCourse->getString("instructor") << '\n';
            }

            delete stmtCourse;
            delete resCourse;
	}
	delete res;
	delete stmt;
    }
    void deleteStudent(sql::Connection* con, std::string index) {
	sql::Statement* stmt;
	stmt = con->createStatement();
	std::string query = "delete from students where student_index = '" + index + "'";

	stmt->execute(query);
	delete stmt;
    }
    void deleteStudent(sql::Connection* con, int student_id) {
        sql::PreparedStatement* stmt = con->prepareStatement("delete from students where student_id = ?");
        stmt->setInt(1, student_id);
        stmt->executeUpdate();

        delete stmt;
    }
    void deleteCourse(sql::Connection* con, std::string course_name) {
	sql::Statement* stmt;
	stmt = con->createStatement();
	std::string query = "delete from courses where course_name = '" + course_name + "'";

	stmt->execute(query);
	delete stmt;
    }
    void deleteCourse(sql::Connection* con, int course_id) {
	sql::Statement* stmt;
	stmt = con->createStatement();
	std::string query = "delete from courses where course_name = '" + std::to_string(course_id)+ "'";

	stmt->execute(query);
	delete stmt;
    }
    void insertNewStudent(sql::Connection *con, const Student& student_ex) {
        sql::Statement *stmt;
        stmt = con->createStatement();
        std::string query = "INSERT INTO students (first_name, last_name, student_index, age) VALUES ('" + student_ex.get_first_name() + "', '" + student_ex.get_last_name() + "', '" + student_ex.get_index() + "', " + std::to_string(student_ex.get_age()) + ")";

        stmt->execute(query);
        
        delete stmt;
    }
    void insertNewCourse(sql::Connection* con, const Courses& course_ex) {
        sql::Statement *stmt;
        stmt = con->createStatement();
        std::string query = "insert into courses (course_name, instructor) values ('" + course_ex.get_name() + "', '" + course_ex.get_instructor() + "')";

        stmt->execute(query);

        delete stmt;
    }
};

#endif // !"ADMIN_HPP"
