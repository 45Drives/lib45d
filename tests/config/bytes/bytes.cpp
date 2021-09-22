/**
 * @code
 */

#include <45d/config/ConfigParser.hpp>
#include <cassert>
#include <iostream>

/**
 * @brief Test implmentation of an ffd::ConfigParser with byte values
 *
 */
class ConfigBytes : public ffd::ConfigParser {
	bool parse_failed_ = false;
public:
	ffd::Bytes bytes_test_ = get<ffd::Bytes>("Bytes Test", &parse_failed_);
	ffd::Bytes kilobytes_test_ = get<ffd::Bytes>("Kilobytes Test", &parse_failed_);
	ffd::Bytes kibibytes_test_ = get<ffd::Bytes>("Kibibytes Test", &parse_failed_);
	ffd::Bytes megabytes_test_ = get<ffd::Bytes>("Megabytes Test", &parse_failed_);
	ffd::Bytes mebibytes_test_ = get<ffd::Bytes>("Mebibytes Test", &parse_failed_);
	ffd::Bytes gigabytes_test_ = get<ffd::Bytes>("Gigabytes Test", &parse_failed_);
	ffd::Bytes gibibytes_test_ = get<ffd::Bytes>("Gibibytes Test", &parse_failed_);

	ConfigBytes(std::string path) : ffd::ConfigParser(path) {
		if (parse_failed_)
			throw ffd::MissingOptionException(
				"################ Config Bytes ################\nOne or more options missing from configuration.");
	}
	void dump(void) {
		std::cout << "Bytes Test: " << bytes_test_.get_str(ffd::Bytes::PrefixType::SI) << std::endl;
		std::cout << "Kilobytes Test: " << kilobytes_test_.get_str(ffd::Bytes::PrefixType::SI)
				  << std::endl;
		std::cout << "Kibibytes Test: " << kibibytes_test_ << std::endl;
		std::cout << "Megabytes Test: " << megabytes_test_.get_str(ffd::Bytes::PrefixType::SI)
				  << std::endl;
		std::cout << "Mebibytes Test: " << mebibytes_test_ << std::endl;
		std::cout << "Gigabytes Test: " << gigabytes_test_.get_str(ffd::Bytes::PrefixType::SI)
				  << std::endl;
		std::cout << "Gibibytes Test: " << gibibytes_test_ << std::endl;
	}
	bool ok(void) {
		return !parse_failed_;
	}
};

int main(int argc, char *argv[]) {
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
