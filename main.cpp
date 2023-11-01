#include "enrollment.hpp"

int menu();
void insertNewStudent(sql::Connection*, const Student&);
void insertNewCourse(sql::Connection*, const Courses&);
void listCourses(sql::Connection*);
void listStudents(sql::Connection*);
void deleteStudentByIndex(sql::Connection*, std::string);
void deleteCourse(sql::Connection*, std::string);
void listEnrollments(sql::Connection*);
void enrollStudentIntoCourse(sql::Connection*, const std::string&, const std::string&);

int main() {
	
	sql::Driver *driver;
	sql::Connection *con;
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "mrda", "mrda");
	con->setSchema("studentMS");

	int x = 0;	
	int number = 0;
	std::string string = "";
	Student student_ex;
	Courses course_ex;
	std::string string2 = "";
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
	std::cout << "";	
    int opcija;
    std::cin >> opcija;

    return opcija;
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
		//TODO
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
