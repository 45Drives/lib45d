#include "45d_conf.hpp"
#include <cassert>
#include <iostream>

/**
 * @brief Test implmentation of an ffd::ConfigParser with unimportant fields
 * 
 */
class ConfigFallback : public ffd::ConfigParser {
public:
	int integer_test_ = get_as<int>("Integer Test", -1);
	unsigned unsigned_test_ = get_as<unsigned>("Unsigned Test", (unsigned)-1);
	double float_test_ = get_as<float>("Float Test", -1.0);
	double double_test_ = get_as<double>("Double Test", -1.0);
	std::string string_test_ = get_as<std::string>("String Test", "fallback");
	ConfigFallback(std::string path) : ffd::ConfigParser(path) {}
	void dump(void) {
		std::cout << "Integer Test: " << integer_test_ << std::endl;
		std::cout << "Unsigned Test: " << unsigned_test_ << std::endl;
		std::cout << "Float Test: " << float_test_ << std::endl;
		std::cout << "Double Test: " << double_test_ << std::endl;
		std::cout << "String Test: " << string_test_ << std::endl;
	}
};

int main (int argc, char *argv[]) {
	assert(argc == 2);
	{
		ConfigFallback config(argv[1]);
		config.dump();
	}
	return 0;
}