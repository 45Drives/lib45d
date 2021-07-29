/**
 * \code
 *
 */

#include "45d_conf.hpp"
#include <cassert>
#include <iostream>

/**
 * @brief Test implmentation of an ffd::ConfigParser with important fields
 * 
 */
class ConfigFailFlag : public ffd::ConfigParser {
	bool parse_failed_ = false;
public:
	int integer_test_ = get_as<int>("Integer Test", parse_failed_);
	unsigned unsigned_test_ = get_as<unsigned>("Unsigned Test", parse_failed_);
	double float_test_ = get_as<float>("Float Test", parse_failed_);
	double double_test_ = get_as<double>("Double Test", parse_failed_);
	std::string string_test_ = get_as<std::string>("String Test", parse_failed_);
	ConfigFailFlag(std::string path) : ffd::ConfigParser(path) {
		if (parse_failed_)
			throw ffd::MissingOptionException("################ Config Fail Flag ################\nOne or more options missing from configuration.");
	}
	void dump(void) {
		std::cout << "Integer Test: " << integer_test_ << std::endl;
		std::cout << "Unsigned Test: " << unsigned_test_ << std::endl;
		std::cout << "Float Test: " << float_test_ << std::endl;
		std::cout << "Double Test: " << double_test_ << std::endl;
		std::cout << "String Test: " << string_test_ << std::endl;
	}
	bool ok(void) {
		return !parse_failed_;
	}
};

int main (int argc, char *argv[]) {
	assert(argc == 2);
	{
		try {
			ConfigFailFlag config(argv[1]);
			config.dump();
		} catch (const ffd::MissingOptionException &err) {
			std::cerr << err.what() << std::endl;
		}
	}
	return 0;
}

/**
 * \endcode
 * 
 */