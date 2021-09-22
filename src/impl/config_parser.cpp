/*
	Copyright (C) 2021 Joshua Boudreau <jboudreau@45drives.com>

	This file is part of lib45d.

	lib45d is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	lib45d is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with lib45d.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "45d/config/ConfigParser.hpp"
#include "local/parser_functions.hpp"

#include <fstream>

ffd::ConfigParser::ConfigParser(std::string path)
	: guarded_(false)
	, config_map_ptr_(&config_map_)
	, path_(path) {
	std::ifstream file(path_);
	if (!file)
		throw(ffd::NoConfigException("No config file at " + path_));
	parse(file);
	file.close();
	config_map_ptr_ = &config_map_; // reset to global
}

void ffd::ConfigParser::parse(std::ifstream &file) {
	std::string line;
	while (getline(file, line)) {
		switch (l::check_record_type(line)) {
			case l::RecordType::UNK:
				std::cerr << "Unknown config entry: " << line << std::endl;
				break;
			case l::RecordType::ENTRY:
				parse_entry(line);
				break;
			case l::RecordType::HEADING:
				parse_heading(line);
				break;
			case l::RecordType::EMPTY:
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
	l::strip_whitespace(key);
	l::remove_comments(value);
	l::strip_whitespace(value);
	config_map_ptr_->insert(std::pair<std::string, ConfigNode>(key, ConfigNode(value, nullptr)));
}

void ffd::ConfigParser::parse_heading(const std::string &line) {
	std::string name = line;
	l::remove_comments(name);
	l::strip_whitespace(name);
	name = name.substr(1, name.length() - 2);
	ConfigNode node(name, new std::unordered_map<std::string, ConfigNode>());
	config_map_ptr_ = node.sub_map_;
	config_map_.insert(std::pair<std::string, ConfigNode>(name, std::move(node)));
	sub_confs_.push_back(&config_map_.at(name));
}

std::string ffd::ConfigParser::dump_str(void) const {
	std::string result;
	for (auto &i : config_map_) {
		if (i.second.sub_map_ == nullptr) {
			result += i.first + " = " + i.second.value_ + '\n';
		} else {
			result += "[" + i.first + "]" + '\n';
			for (auto &j : *i.second.sub_map_) {
				result += "  " + j.first + " = " + j.second.value_ + '\n';
			}
		}
	}
	return result;
}

template<>
bool ffd_internal::get<bool>(const std::string &key,
							 const std::unordered_map<std::string, ffd::ConfigNode> *config_map) {
	ffd::ConfigNode node = config_map->at(key);
	std::stringstream ss(node.value_);
	ss.exceptions(std::ios::failbit | std::ios::badbit);
	bool result;
	ss >> std::boolalpha >> result;
	return result;
}

template<>
std::string
ffd_internal::get<std::string>(const std::string &key,
							   const std::unordered_map<std::string, ffd::ConfigNode> *config_map) {
	ffd::ConfigNode node = config_map->at(key);
	std::stringstream ss(node.value_);
	ss.exceptions(std::ios::failbit | std::ios::badbit);
	std::string result;
	getline(ss, result);
	return result;
}
