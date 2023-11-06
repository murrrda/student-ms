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
		Student(std::string username, std::string password, std::string first_name = "fake first name", std::string last_name = "fake last name", std::string index = "INXX-YYYY", int age = 0): User(username, password), first_name(first_name), last_name(last_name), index(index), age(age) {}

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
