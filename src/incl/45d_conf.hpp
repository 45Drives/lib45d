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

#define USE_BOOST

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef USE_BOOST
#include <boost/lexical_cast.hpp>
#endif

/**
 * @brief 45Drives Namespace
 * 
 */
namespace ffd {
	/** Exceptions
	 * @brief Exceptions thrown by this library
	 * 
	 */
	class ConfigException {
	private:
		std::string what_; ///< String containing explanation message
	public:
		/**
		 * @brief Construct a new Exception object
		 * 
		 * @param what String containing explanation message
		 */
		ConfigException(std::string what) : what_(what) {}
		/**
		 * @brief Return string containing explanation message
		 * 
		 * @return std::string 
		 */
		std::string what(void) const {
			return what_;
		}
	};

	/**
	 * @brief Throw this exception when the config file fails to open
	 * 
	 */
	class NoConfigException : public ConfigException {
	public:
		NoConfigException(std::string what) : ConfigException(what) {}
	};

	/**
	 * @brief Throw this exception when a config entry is missing
	 * 
	 */
	class MissingOptionException : public ConfigException {
	public:
		MissingOptionException(std::string what) : ConfigException(what) {}
	};

	/**
	 * @brief Struct for config_map_ entries
	 * 
	 */
	class Node {
	public:
		std::string value_ = ""; ///< string from config file after '='
		std::unordered_map<std::string, Node> *sub_map_ = nullptr; ///< Pointer to submap for config sections
		/**
		 * @brief Construct a new Node object
		 * 
		 * @param value String containing config value or subsection header
		 * @param sub_map nullptr or pointer to an std::unordered_map<std::string, ffd::Node> if subsection
		 */
		explicit Node(std::string value, std::unordered_map<std::string, Node> *sub_map) : value_(value), sub_map_(sub_map) {}
		/**
		 * @brief Destroy the Node object
		 * Deletes the sub_map_ member if allocated
		 */
		Node() {}
		Node(const Node &other) {
			value_ = other.value_;
			sub_map_ = other.sub_map_;
		}
		~Node() {
			if (sub_map_)
				delete sub_map_;
		}
	};

	/**
	 * @brief Get config entry as type T from configuration map
	 * 
	 * @tparam T Type of variable to return
	 * @param key Key to index map, from config file before '='
	 * @param config_map Map of keys to Node structs
	 * @return T Value returned from config
	 */
	template<class T>
	T get_as(const std::string &key, const std::unordered_map<std::string, Node> *config_map) {
		std::stringstream ss;
					Node node = config_map->at(key);
		#ifdef USE_BOOST
					T result = boost::lexical_cast<T>(node.value_);
		#else
					ss.str(node.value_);
					T result;
					ss >> result;
		#endif
					return result;
	}

	// template<>
	// std::string get_as<std::string>(const std::string &key, const std::unordered_map<std::string, Node> *config_map) {
	// 	std::stringstream ss;
	// 				Node node = config_map->at(key);
	// 	#ifdef USE_BOOST
	// 				std::string result = boost::lexical_cast<std::string>(node.value_);
	// 	#else
	// 				ss.str(node.value_);
	// 				T result;
	// 				ss >> result;
	// 	#endif
	// 				return result;
	// }
	/**
	 * @brief Main configuration parser class to inherit from in your code  
	 * Example usage:
	 * \include tests/test.cpp
	 * 
	 */
	class ConfigParser {
	private:
		std::unordered_map<std::string, Node> *config_map_ptr_;
		std::string path_; ///< Path to config file
		std::unordered_map<std::string, Node> config_map_; ///< Map of config keys to values (Node)
		std::vector<Node *> sub_confs_; ///< Vector of config subsections
		/**
		 * @brief Iterate each line of config file and determine how to parse with check_record_type()
		 * 
		 * @param file Opened file stream for config file
		 */
		void parse(std::ifstream &file);
		/**
		 * @brief Extract value from config line and insert node into config_map_
		 * 
		 * @param line String containing some form of "key = value"
		 */
		void parse_entry(const std::string &line);
		/**
		 * @brief Extract name of subsection and create new Node containing new config map in vector, and assigned config_map_ptr_ to address of new map
		 * 
		 * @param line String containing some form of "[Section Name]"
		 */
		void parse_heading(const std::string &line);
	public:
		/**
		 * @brief Construct a new Config Parser object
		 * 
		 * @param path Path to config file
		 */
		ConfigParser(std::string path);
		/**
		 * @brief Dump config to stdout as a string
		 * 
		 * @return std::string String containing config_map_ contents
		 */
		std::string dump_str(void) const;
		/**
		 * @brief Adapter for ffd::get_as(). This can throw.
		 * 
		 * @tparam T Type of variable to get
		 * @param key Key to index config_map_
		 * @return T Value returned from config
		 */
		template<class T>
		T get_as(const std::string &key) const {
			return ffd::get_as<T>(key, config_map_ptr_);
		}
		/**
		 * @brief Try to get value from config, default to fallback if fails. Guaranteed no-throw.
		 * 
		 * @tparam T Type of variable to return
		 * @param key Key to index config_map_
		 * @param fallback Default value to return if ffd::get_as() fails.
		 * @return T Value returned from config
		 */
		template<class T>
		T get_as(const std::string &key, const T &fallback) const noexcept {
			try {
				return get_as<T>(key);
#ifdef USE_BOOST
			} catch (const boost::bad_lexical_cast &) {
#else
			} catch (const std::invalid_argument &) {
#endif
				return fallback;
			} catch (const std::out_of_range &) {
				return fallback;
			}
		}
		/**
		 * @brief Try to get value from config. If ffd::get fails, return T() or 0 and set fail_flag. Guaranteed no-throw.
		 * 
		 * @tparam T Type of variable to return
		 * @param key Key to index config_map_
		 * @param fail_flag Flag to set if ffd::get_as() fails
		 * @return T Value returned from config
		 */
		template<class T>
		T get_as(const std::string &key, bool &fail_flag) const noexcept{
			try {
				return get_as<T>(key);
#ifdef USE_BOOST
			} catch (const boost::bad_lexical_cast &) {
#else
			} catch (const std::invalid_argument &) {
#endif
				std::cerr << "Invalid configuration entry format: " << key << " = " << config_map_.at(key).value_ << std::endl;
				fail_flag = true;
				if (std::is_fundamental<T>::value)
					return 0;
				else
					return T();
			} catch (const std::out_of_range &) {
				std::cerr << "Option not in config: " << key << std::endl;
				fail_flag = true;
				if (std::is_fundamental<T>::value)
					return 0;
				else
					return T();
			}
		}
	};
}
