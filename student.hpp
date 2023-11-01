#ifndef STUDENT_HPP
#define STUDENT_HPP

class Student {
	private:
		int id;							//database identifier
		
		std::string first_name;
		std::string last_name;
		std::string index;
		int age;
	public:
		Student(std::string first_name = "fake first name", std::string last_name = "fake last name", std::string index = "INXX-YYYY", int age = 0) {
			this->first_name = first_name;
			this->last_name = last_name;
			this->index = index;
			this->age = age;
		}
		Student(const Student& student){
			id = student.id;
			first_name = student.first_name;
			last_name = student.last_name;
			index = student.index;
			age = student.age;
		}
		
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
		
		void set_id(int new_id) {
			id = new_id;
		}
		void set_first_name(std::string new_name) {
			first_name = new_name;
		}
		
		void set_last_name(std::string new_name) {
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
};

#endif
