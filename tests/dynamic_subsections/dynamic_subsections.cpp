#include "45d_conf.hpp"
#include <cassert>
#include <iostream>
#include <vector>

class SubConf {
public:
	std::string name_;
	bool bool_test_;
	int integer_test_;
	unsigned unsigned_test_;
	float float_test_;
	double double_test_;
	std::string string_test_;
	void dump(void) {
		std::cout << name_ << " Bool Test: " << std::boolalpha << bool_test_ << std::endl;
		std::cout << name_ << " Integer Test: " << integer_test_ << std::endl;
		std::cout << name_ << " Unsigned Test: " << unsigned_test_ << std::endl;
		std::cout << name_ << " Float Test: " << float_test_ << std::endl;
		std::cout << name_ << " Double Test: " << double_test_ << std::endl;
		std::cout << name_ << " String Test: " << string_test_ << std::endl;
	}
};

class DynamicSubsectionConfig : public ffd::ConfigParser {
private:
public:
	bool bool_test_ = get<bool>("Bool Test", false);
	int integer_test_ = get<int>("Integer Test", -1);
	unsigned unsigned_test_ = get<unsigned>("Unsigned Test", (unsigned)-1);
	float float_test_ = get<float>("Float Test", -1.0);
	double double_test_ = get<double>("Double Test", -1.0);
	std::string string_test_ = get<std::string>("String Test", "fallback");
	DynamicSubsectionConfig(const std::string &path, std::vector<SubConf> &sub_confs) : ConfigParser(path) {
		for (ffd::Node *i : sub_confs_) {
			{
				ffd::ConfigSubsectionGuard guard(*this, i->value_);
				sub_confs.emplace_back(SubConf{
					i->value_,
					get<bool>("Bool Test", false),
					get<int>("Integer Test", -1),
					get<unsigned>("Unsigned Test", (unsigned)-1),
					get<float>("Float Test", -1.0),
					get<double>("Double Test", -1.0),
					get<std::string>("String Test", "fallback")
				});
			}
		}
	}
	void dump(void) {
		std::cout << "Global Bool Test: " << std::boolalpha << bool_test_ << std::endl;
		std::cout << "Global Integer Test: " << integer_test_ << std::endl;
		std::cout << "Global Unsigned Test: " << unsigned_test_ << std::endl;
		std::cout << "Global Float Test: " << float_test_ << std::endl;
		std::cout << "Global Double Test: " << double_test_ << std::endl;
		std::cout << "Global String Test: " << string_test_ << std::endl;
	}
};

int main(int argc, char *argv[]) {
	assert(argc == 2);
	std::vector<SubConf> sub_confs;
	DynamicSubsectionConfig config(argv[1], sub_confs);
	config.dump();
	for (auto i : sub_confs)
		i.dump();
	return 0;
	return 0;
}