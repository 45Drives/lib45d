#include "45d_conf.hpp"
#include "parser_functions.hpp"

#include <iostream>

ffd::ConfigParser::ConfigParser(std::string path) : path_(path) {
	std::ifstream file(path_);
	if (!file)
		throw(ffd::NoConfigException("No config file at " + path_));
	parse(file);
	file.close();
}

void ffd::ConfigParser::parse(std::ifstream &file) {
	std::string line;
	while (getline(file, line)) {
		switch(check_record_type(line)) {
			case RecordType::UNK:
				std::cerr << "Unknown config entry: " << line << std::endl;
				break;
			case RecordType::ENTRY:
				parse_entry(line);
				break;
			case RecordType::HEADING:

				break;
			case RecordType::EMPTY:
			default:
				break;
		}
	}
}

void ffd::ConfigParser::parse_entry(const std::string &line) {
	std::string key, value;
	std::stringstream ss(line);
	getline(ss, key, '=');
	getline(ss, value);
	strip_whitespace(key);
	remove_comments(value);
	strip_whitespace(value);
	config_map_[key] = value;
}

std::string ffd::ConfigParser::dump_str(void) const {
	std::string result;
	for (auto &i : config_map_)
		result += i.first + " = " + i.second + '\n';
	return result;
}
