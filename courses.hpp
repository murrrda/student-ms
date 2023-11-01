#ifndef COURSES_HPP
#define COURSES_HPP

class Courses{
	private:
		int course_id;
		std::string name;
		std::string instructor;
	public:
		Courses(std::string name = "fake name", std::string instructor = "fake instructor") {
			
			this->name = name;
			this->instructor = instructor;
		}
		Courses(const Courses& course) {
			course_id = course.course_id;
			name = course.name;
			instructor = course.instructor;
		}
		
		std::string get_name() const {
			return name;
		}
		std::string get_instructor() const {
			return instructor;
		}
		
		void set_id(int new_id){
			course_id = new_id;
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
