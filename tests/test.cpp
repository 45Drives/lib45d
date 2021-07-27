#include "45d_conf.hpp"
#include <cassert>
#include <iostream>

class Config : public ffd::ConfigParser {
public:
	bool parse_failed = false;
	long integer_test_ = get<long>("Integer Test", parse_failed);
	double double_test_ = get<double>("Double Test", parse_failed);
	std::string string_test_ = get<std::string>("String Test", parse_failed);
	Config(std::string path) : ffd::ConfigParser(path) {
		if (parse_failed)
			throw ffd::MissingOptionException("One or more options missing from configuration.");
	}
	void dump(void) {
		std::cout << "Integer Test: " << integer_test_ << std::endl;
		std::cout << "Double Test: " << double_test_ << std::endl;
		std::cout << "String Test: " << string_test_ << std::endl;
	}
};

int main (int argc, char *argv[]) {
	assert(argc == 2);
	Config *config;
	try {
		config = new Config(argv[1]);
	} catch (const ffd::MissingOptionException &err) {
		std::cerr << err.what() << std::endl;
		return -1;
	}
	config->dump();
	delete config;
	return 0;
}