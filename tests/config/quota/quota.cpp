/**
 * @code
 */

#include <45d/config/ConfigParser.hpp>
#include <45d/Quota.hpp>
#include <cassert>
#include <iostream>

/**
 * @brief Test implmentation of an ffd::ConfigParser with quotas
 * 
 */
class ConfigBytes : public ffd::ConfigParser {
	bool parse_failed_ = false;
public:
	ffd::Quota percent_;
    ffd::Quota fraction_;
	
	ConfigBytes(std::string path) : ffd::ConfigParser(path) {
        ffd::Bytes total_bytes = get<ffd::Bytes>("Total Size", &parse_failed_);
        percent_ = get_quota("Percent", total_bytes, &parse_failed_);
        fraction_ = get_quota("Fraction", total_bytes, &parse_failed_);
		if (parse_failed_)
			throw ffd::MissingOptionException("################ Config Quota ################\nOne or more options missing from configuration.");
	}
	void dump(void) {
		std::cout << "Percent: " << percent_.get_str(ffd::Bytes::PrefixType::SI) << std::endl;
		std::cout << "Fraction: " << fraction_.get_str(ffd::Bytes::PrefixType::SI) << std::endl;
	}
	bool ok(void) {
		return !parse_failed_;
	}
};

int main (int argc, char *argv[]) {
	assert(argc == 2);
	{
		try {
			ConfigBytes config(argv[1]);
			config.dump();
		} catch (const ffd::MissingOptionException &err) {
			std::cerr << err.what() << std::endl;
		}
	}
	return 0;
}

/**
 * @endcode
 */