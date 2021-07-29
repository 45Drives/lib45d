#include "45d_conf.hpp"
#include <cassert>
#include <iostream>

class ConfigFailFlag : public ffd::ConfigParser {
	bool parse_failed_ = false;
public:
	int integer_test_ = get<int>("Integer Test", parse_failed_);
	unsigned unsigned_test_ = get<unsigned>("Unsigned Test", parse_failed_);
	double float_test_ = get<float>("Float Test", parse_failed_);
	double double_test_ = get<double>("Double Test", parse_failed_);
	std::string string_test_ = get<std::string>("String Test", parse_failed_);
	ConfigFailFlag(std::string path) : ffd::ConfigParser(path) {
		if (parse_failed_)
			throw ffd::MissingOptionException("################ Config Fail Flag ################\nOne or more options missing from configuration.");
	}
	void dump(void) {
		std::cout << "################ Config Fail Flag ################" << std::endl;
		if (parse_failed_) {
			std::cout << "PARSE FAILED" << std::endl;
		} else {
			std::cout << "Integer Test: " << integer_test_ << std::endl;
			std::cout << "Unsigned Test: " << unsigned_test_ << std::endl;
			std::cout << "Float Test: " << float_test_ << std::endl;
			std::cout << "Double Test: " << double_test_ << std::endl;
			std::cout << "String Test: " << string_test_ << std::endl;
		}
	}
	bool ok(void) {
		return !parse_failed_;
	}
};

class ConfigFallback : public ffd::ConfigParser {
public:
	int integer_test_ = get<int>("Integer Test", -1);
	unsigned unsigned_test_ = get<unsigned>("Unsigned Test", (unsigned)-1);
	double float_test_ = get<float>("Float Test", -1.0);
	double double_test_ = get<double>("Double Test", -1.0);
	std::string string_test_ = get<std::string>("String Test", "fallback");
	ConfigFallback(std::string path) : ffd::ConfigParser(path) {}
	void dump(void) {
		std::cout << "################ Config Fallback ################" << std::endl;
		std::cout << "Integer Test: " << integer_test_ << std::endl;
		std::cout << "Unsigned Test: " << unsigned_test_ << std::endl;
		std::cout << "Float Test: " << float_test_ << std::endl;
		std::cout << "Double Test: " << double_test_ << std::endl;
		std::cout << "String Test: " << string_test_ << std::endl;
	}
};

int main (int argc, char *argv[]) {
	assert(argc == 2);
	std::cout << "################ Testing with " << argv[1] << " ################" << std::endl;
	{
		try {
			ConfigFailFlag config(argv[1]);
			config.dump_str();
			config.dump();
		} catch (const ffd::MissingOptionException &err) {
			std::cerr << err.what() << std::endl;
		}
	}
	{
		ConfigFallback config(argv[1]);
		config.dump_str();
		config.dump();
	}
	std::cout << std::endl;
	return 0;
}