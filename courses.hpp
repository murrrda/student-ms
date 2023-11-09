#ifndef COURSES_HPP
#define COURSES_HPP

class Courses{
	private:
		std::string name;
		std::string instructor;
	public:
		Courses(const std::string& name = "fake name", const std::string& instructor = "fake instructor") {
			this->name = name;
			this->instructor = instructor;
		}
		Courses(const Courses& course) {
			name = course.name;
			instructor = course.instructor;
		}
		
		std::string get_name() const {
			return name;
		}
		std::string get_instructor() const {
			return instructor;
		}
		void set_name(std::string new_name) {
			name = new_name;
		}
		void set_instructor(std::string new_instructor) {
			instructor = new_instructor;
		}
		
		void print_course_info() const {
			std::cout << '\n';
			std::cout << "Course name: "<< name << '\n';
			std::cout << "Instructor: " << instructor << '\n' << '\n';
		}
};
#endif
