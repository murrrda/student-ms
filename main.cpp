#include "enrollment.hpp"
#include "config.h"

int menu();
void insertNewStudent(sql::Connection*, const Student&);
void insertNewCourse(sql::Connection*, const Courses&);
void listCourses(sql::Connection*);
void listStudents(sql::Connection*);
void deleteStudentByIndex(sql::Connection*, std::string);
void deleteCourse(sql::Connection*, std::string);
void listEnrollments(sql::Connection*);
void enrollStudentIntoCourse(sql::Connection*, const std::string&, const std::string&);
int updateStudentInfoMenu();
void updateStudentFirstName(sql::Connection*, int, const std::string&);
void updateStudentLastName(sql::Connection*, int, const std::string&);
void updateStudentIndex(sql::Connection*, int, const std::string&);
int getStudentIdByIndex(sql::Connection*, const std::string&);
void updateStudentByAge(sql::Connection*, int, int);

int main() {
	
	sql::Driver *driver;
	sql::Connection *con;
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", USERNAME, PASSWORD);
	con->setSchema("studentMS");

	int x = 0;	
	int number = 0;
	std::string string = "";
	Student student_ex;
	Courses course_ex;
	std::string string2 = "";
	int number2 = 0;
	int newAge = 0;
	
	do{
		x = menu();
		switch(x) {
			case 0:
				std::cout << "Exiting...\n";
				break;
			case 1: 
				std::cout << "Enter your first name: ";
				std::cin >> string;
				student_ex.set_first_name(string);
				std::cout << "Enter your last name: ";
				std::cin >> string;
				student_ex.set_last_name(string);
				std::cout << "Enter your index: ";
				std::cin >> string;
				student_ex.set_index(string);
				std::cout << "Enter your age: ";
				std::cin >> number;
				student_ex.set_age(number);
				insertNewStudent(con, student_ex);
				break;
			case 2:
				std::cout << "Enter course name: ";
				std::cin >> string;
				course_ex.set_name(string);
				std::cout << "Enter course instructor: ";
				std::cin >> string;
				course_ex.set_instructor(string);
				insertNewCourse(con, course_ex);
				break;
			case 3:
				listCourses(con);
				break;
			case 4:
				listStudents(con);
				break;
			case 5:
				std::cout << "Enter students index: ";
				std::cin >> string;
				deleteStudentByIndex(con, string);
				break;
			case 6:
				std::cout << "Enter course name: ";
				std::cin >> string;
				deleteCourse(con, string);
				break;
			case 7:
				listEnrollments(con);
				break;
			case 8:
				std::cout << "Enter students index: ";
				std::cin >> string;
				std::cout << "\nEnter course name: ";
				std::cin >> string2;
				enrollStudentIntoCourse(con, string, string2);
				break;
			case 9:
				do{
				std::cout << "Enter student's Index: ";
				std::cin >> string2;
				number2 = getStudentIdByIndex(con, string2);
				} while(number2 == 0);
				do{
				number = updateStudentInfoMenu(); 
					switch(number) {
						case 0:
							std::cout << "Exiting...\n";
							break;
						case 1: 
							std::cout << "Enter new first name: ";
							std::cin >> string;
							updateStudentFirstName(con, number2, string);
							break;
						case 2: 
							std::cout << "Enter new last name: ";
							std::cin >> string;
							updateStudentLastName(con, number2, string);
							break;
						case 3: 
							std::cout << "Enter new index: ";
							std::cin >> string;
							updateStudentIndex(con, number2, string);
							break;						
						case 4: 
							std::cout << "Enter new age: ";
							std::cin >> newAge;
							updateStudentByAge(con, number2, newAge);
							break;
						default:
							std::cout << "Wrong option\n";
							break;
					}
				} while(number != 0);
			default:
				std::cout << "You chose the wrong option!\n";
		}
		
	}while(x != 0);

	delete con;
	return 0;
}

int menu() {
    std::cout << "\n\t Choose one option:\n";
	std::cout << "\t\t0. Exit program\n";
    std::cout << "\t\t1. Enter new student\n";
    std::cout << "\t\t2. Enter new cours\n";
    std::cout << "\t\t3. List courses\n";
	std::cout << "\t\t4. List students\n";
	std::cout << "\t\t5. Delete student by Index\n";
	std::cout << "\t\t6. Delete course\n";
	std::cout << "\t\t7. List enrollments\n";
	std::cout << "\t\t8. Enroll student into course\n";
	std::cout << "\t\t9. Update student information\n";
	std::cout << "";	
    int opcija;
    std::cin >> opcija;

    return opcija;
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
void listCourses(sql::Connection* con) {
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
void listStudents(sql::Connection* con) {
	sql::Statement *stmt;
	sql::ResultSet *res;
	stmt = con->createStatement();
	int i = 0;	
	std::cout << '\n';
	res = stmt->executeQuery("select first_name, last_name, student_index, age from students");
	while(res->next()) {
		i++;
		std::cout << i << "." << '\t'  << "First name: " << res->getString("first_name");
		std::cout << ", Second name: " << res->getString("last_name");	
		std::cout << ", Index: " << res->getString("student_index");
		std::cout << ", Age: " << res->getInt("age");
		std::cout << '\n';
	}
	
	delete res;
	delete stmt;
}

void deleteStudentByIndex(sql::Connection* con, std::string index) {
	sql::Statement* stmt;
	stmt = con->createStatement();
	std::string query = "delete from students where student_index = '" + index + "'";

	stmt->execute(query);
	delete stmt;
}
void deleteCourse(sql::Connection* con, std::string course_name) {
	sql::Statement* stmt;
	stmt = con->createStatement();
	std::string query = "delete from courses where course_name = '" + course_name + "'";

	stmt->execute(query);
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
void enrollStudentIntoCourse(sql::Connection* con, const std::string& index, const std::string& course_name) {
	try{
		sql::PreparedStatement* stmtStudent = con->prepareStatement("select student_id from students where student_index = ?");
		stmtStudent->setString(1, index);
		
		sql::ResultSet* resStudentId = stmtStudent->executeQuery();
		
		int student_id = 0;
		if(resStudentId->next())
			student_id = resStudentId->getInt("student_id");

		delete stmtStudent;
		delete resStudentId;

		sql::PreparedStatement* stmtCourse = con->prepareStatement("select course_id from courses where course_name = ?");
		stmtCourse->setString(1, course_name);
		sql::ResultSet* resCourseId = stmtCourse->executeQuery();
		
		int course_id = 0;
		if(resCourseId->next())
			course_id = resCourseId->getInt("course_id");

		delete stmtCourse;
		delete resCourseId;
		
		if(student_id != 0 && course_id != 0) {
		sql::PreparedStatement* stmtEnrollment = con->prepareStatement("insert into enrollment (student_id, course_id) values (?, ?)");
		stmtEnrollment->setInt(1, student_id);
		stmtEnrollment->setInt(2, course_id);
		stmtEnrollment->executeQuery();
		}
		else
			std::cerr << "Enrollment failed, student or course not found!\n";
	} catch (sql::SQLException &e) {
        std::cerr << "Enrollment failed: " << e.what() << std::endl;
    }
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

int getStudentIdByIndex(sql::Connection* con, const std::string& index) {
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
void updateStudentByAge(sql::Connection* con, int student_id, int age) {
	if(student_id != 0) {
		sql::PreparedStatement* stmtName = con->prepareStatement("UPDATE students SET age = ? WHERE student_id = ?");
		stmtName->setInt(1, age);
		stmtName->setInt(2, student_id);
		stmtName->executeUpdate();

		delete stmtName;
	}
}
