/*
 *    Copyright (C) 2021 Joshua Boudreau <jboudreau@45drives.com>
 *    
 *    This file is part of lib45d_conf.
 * 
 *    lib45d_conf is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 * 
 *    lib45d_conf is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with lib45d_conf.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>

namespace ffd {
	class Exception {
	private:
		std::string what_;
	public:
		Exception(std::string what) : what_(what) {}
		std::string what(void) const {
			return what_;
		}
	};

	class NoConfigException : public Exception {
	public:
		NoConfigException(std::string what) : Exception(what) {}
	};

	class MissingOptionException : public Exception {
	public:
		MissingOptionException(std::string what) : Exception(what) {}
	};

	class ConfigParser {
	private:
		std::string path_;
		std::unordered_map<std::string, std::string> config_map_;
		void parse(std::ifstream &file);
		void parse_entry(const std::string &line);
	public:
		ConfigParser(std::string path);
		std::string dump_str(void) const;
		template<class T>
		T get(const std::string &key) const {
			std::stringstream ss;
			ss.str(config_map_.at(key));
			T result;
			ss >> result;
			return result;
		}

		// template<>
		// std::string get<std::string>(const std::string &key) const {
		// 	return config_map_.at(key);
		// }

		template<class T>
		T get(const std::string &key, const T &fallback) const noexcept {
			try {
				return get<T>(key);
			} catch (const std::invalid_argument &) {
				return fallback;
			} catch (const std::out_of_range &) {
				return fallback;
			}
		}

		template<class T>
		T get(const std::string &key, bool &fail_flag) const {
			try {
				return get<T>(key);
			} catch (const std::invalid_argument &) {
				std::cerr << "Invalid configuration entry format: " << key << " = " << config_map_.at(key) << std::endl;
				fail_flag = true;
				return T();
			} catch (const std::out_of_range &) {
				std::cerr << "Option not in config: " << key << std::endl;
				fail_flag = true;
				return T();
			}
		}
	};
};
