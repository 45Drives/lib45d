#include "45d_conf.hpp"
#include <cassert>
#include <iostream>

/**
 * @brief Test implmentation of an ffd::ConfigParser with important fields
 * 
 */
class ConfigSubSections : public ffd::ConfigParser {
private:
	bool parse_failed_ = false;
public:
	bool global_bool_test_ = get<bool>("Bool Test", &parse_failed_);
	int global_integer_test_ = get<int>("Integer Test", &parse_failed_);
	unsigned global_unsigned_test_ = get<unsigned>("Unsigned Test", &parse_failed_);
	float global_float_test_ = get<float>("Float Test", &parse_failed_);
	double global_double_test_ = get<double>("Double Test", &parse_failed_);
	std::string global_string_test_ = get<std::string>("String Test", &parse_failed_);

	bool section1_bool_test_ = get_from<bool>("Section 1", "Bool Test", &parse_failed_);
	int section1_integer_test_ = get_from<int>("Section 1", "Integer Test", &parse_failed_);
	unsigned section1_unsigned_test_ = get_from<unsigned>("Section 1", "Unsigned Test", &parse_failed_);
	float section1_float_test_ = get_from<float>("Section 1", "Float Test", &parse_failed_);
	double section1_double_test_ = get_from<double>("Section 1", "Double Test", &parse_failed_);
	std::string section1_string_test_ = get_from<std::string>("Section 1", "String Test", &parse_failed_);

	bool section2_bool_test_ = get_from<bool>("Section 2", "Bool Test", &parse_failed_);
	int section2_integer_test_ = get_from<int>("Section 2", "Integer Test", &parse_failed_);
	unsigned section2_unsigned_test_ = get_from<unsigned>("Section 2", "Unsigned Test", &parse_failed_);
	float section2_float_test_ = get_from<float>("Section 2", "Float Test", &parse_failed_);
	double section2_double_test_ = get_from<double>("Section 2", "Double Test", &parse_failed_);
	std::string section2_string_test_ = get_from<std::string>("Section 2", "String Test", &parse_failed_);
	ConfigSubSections(std::string path) : ffd::ConfigParser(path) {}
	void dump(void) {
		std::cout << "Global Bool Test: " << std::boolalpha << global_bool_test_ << std::endl;
		std::cout << "Global Integer Test: " << global_integer_test_ << std::endl;
		std::cout << "Global Unsigned Test: " << global_unsigned_test_ << std::endl;
		std::cout << "Global Float Test: " << global_float_test_ << std::endl;
		std::cout << "Global Double Test: " << global_double_test_ << std::endl;
		std::cout << "Global String Test: " << global_string_test_ << std::endl;

		std::cout << "Section 1 Bool Test: " << std::boolalpha << section1_bool_test_ << std::endl;
		std::cout << "Section 1 Integer Test: " << section1_integer_test_ << std::endl;
		std::cout << "Section 1 Unsigned Test: " << section1_unsigned_test_ << std::endl;
		std::cout << "Section 1 Float Test: " << section1_float_test_ << std::endl;
		std::cout << "Section 1 Double Test: " << section1_double_test_ << std::endl;
		std::cout << "Section 1 String Test: " << section1_string_test_ << std::endl;

		std::cout << "Section 2 Bool Test: " << std::boolalpha << section2_bool_test_ << std::endl;
		std::cout << "Section 2 Integer Test: " << section2_integer_test_ << std::endl;
		std::cout << "Section 2 Unsigned Test: " << section2_unsigned_test_ << std::endl;
		std::cout << "Section 2 Float Test: " << section2_float_test_ << std::endl;
		std::cout << "Section 2 Double Test: " << section2_double_test_ << std::endl;
		std::cout << "Section 2 String Test: " << section2_string_test_ << std::endl;
	}
};

int main (int argc, char *argv[]) {
	assert(argc == 2);
	ConfigSubSections config(argv[1]);
	config.dump();
	return 0;
}