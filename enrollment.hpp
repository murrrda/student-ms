#ifndef ENROLLMENT_HPP
#define ENROLLMENT_HPP

#include <iostream>

#include "student.hpp"
#include "courses.hpp"


#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class Enrollment {
	private:
		int enrollment_id;
		int course_id;
		int student_id;
	public:
		Enrollment(int course_id = 0, int student_id = 0) {
			this->course_id = course_id;
			student_id = student_id;
		}
		Enrollment(const Enrollment& new_enrollment) {
			enrollment_id = new_enrollment.enrollment_id;
			course_id = new_enrollment.course_id;
			student_id = new_enrollment.student_id;
		}
		
		int get_enrollment_id() const {
			return enrollment_id;
		}
		int get_course_id() const {
			return course_id;
		}
		int get_student_id() const {
			return student_id;
		}
		
		void set_enrollment_id(int new_enrollment_id) {
			enrollment_id = new_enrollment_id;
		}
		void set_course_id(int new_course_id) {
			course_id = new_course_id;
		}
		void set_student_id(int new_student_id) {
			student_id = new_student_id;
		}
		
		void print_info() const {
			std::cout << '\n';
		}
};

#endif
