// <45dconf> -*- C++ -*-
/*
 *    Copyright (C) 2021 Joshua Boudreau <jboudreau@45drives.com>
 *    
 *    This file is part of lib45dconf.
 * 
 *    lib45dconf is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 * 
 *    lib45dconf is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with lib45dconf.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/exception/all.hpp>

/**
 * @brief 45Drives Namespace
 * 
 */
namespace ffd {
	/** Exceptions
	 * @brief Exceptions thrown by this library
	 * 
	 */
	class ConfigException : public std::exception {
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
		const char *what(void) const noexcept {
			return what_.c_str();
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
	 * @brief Throw this exception when a ConfigGuard is constructed 
	 * or get_from() is called when the config is already guarded
	 * 
	 */
	class ConfigGuardException : public ConfigException {
	public:
		ConfigGuardException(std::string what) : ConfigException(what) {}
	};

	/**
	 * @brief Throw this exception when Bytes::set() fails to parse string
	 * 
	 */
	class ByteParseException : public ConfigException {
	private:
		std::string what_; ///< String containing explanation message
	public:
		ByteParseException(std::string what) : ConfigException(what) {}
	};

	class Bytes {
	private:
		uintmax_t bytes_;
	public:
		/**
		 * @brief Construct a new Bytes object from formatted string
		 * 
		 * @param str formatted string for bytes
		 */
		Bytes(const std::string &str);
		/**
		 * @brief Construct a new Bytes object from integral type
		 * 
		 * @param bytes 
		 */
		Bytes(uintmax_t bytes) : bytes_(bytes) {}
		/**
		 * @brief Construct a new empty Bytes object
		 * 
		 */
		Bytes(void) : bytes_(0) {}
		/**
		 * @brief Copy construct a new Bytes object
		 * 
		 * @param other Bytes to be copied
		 */
		Bytes(const Bytes &other) : bytes_(other.bytes_) {}
		/**
		 * @brief Move constructor
		 * 
		 * @param other Bytes to be moved
		 */
		Bytes(Bytes &&other) : bytes_(std::move(other.bytes_)) {}
		/**
		 * @brief Copy assignment
		 * 
		 * @param other Bytes to be copied
		 * @return Bytes& *this
		 */
		Bytes &operator=(const Bytes &other) {
			bytes_ = other.bytes_;
			return *this;
		}
		/**
		 * @brief Assignment move constructor
		 * 
		 * @param other Bytes to be moved
		 * @return Bytes& *this
		 */
		Bytes &operator=(Bytes &&other) {
			bytes_ = std::move(other.bytes_);
			return *this;
		}
		/**
		 * @brief Destroy the Bytes object
		 */
		~Bytes() = default;
		/**
		 * @brief Get value in bytes
		 * 
		 * @return uintmax_t 
		 */
		uintmax_t get(void) const;
		/**
		 * @brief Get value as formatted string
		 * 
		 * @param base1024 false for base 1000 output, true for base 1024 output
		 * 
		 * @return std::string 
		 */
		std::string get_str(bool base1024 = true) const;
		/**
		 * @brief Set value from integral type
		 * 
		 * @param val 
		 */
		void set(uintmax_t val) {
			bytes_ = val;
		}
		/**
		 * @brief Set value from formatted string
		 * 
		 * @param str 
		 */
		void set(const std::string &str);
		/**
		 * @brief Stream insertion operator
		 * 
		 * @param os 
		 * @param bytes 
		 * @return std::ostream& 
		 */
		friend std::ostream& operator<<(std::ostream& os, Bytes const &bytes) {
			return os << bytes.get_str();
		}
		/**
		 * @brief Stream extraction operator
		 * 
		 * @param is 
		 * @param bytes 
		 * @return std::istream& 
		 */
		friend std::istream& operator>>(std::istream& is, Bytes &bytes) {
			std::string str;
			is >> str;
			bytes.set(str);
			return is;
		}
	};

	
	/**
	 * @brief Struct for config_map_ entries
	 * 
	 */
	class ConfigNode {
	private:
		bool is_copy_; ///< Set in copy constructor to avoid double deletion of *sub_map_
	public:
		std::string value_; ///< string from config file after '='
		std::unordered_map<std::string, ConfigNode> *sub_map_; ///< Pointer to submap for config sections
		/**
		 * @brief Construct a new ConfigNode object
		 * 
		 * @param value String containing config value or subsection header
		 * @param sub_map nullptr or pointer to an std::unordered_map<std::string, ffd::ConfigNode> if subsection
		 */
		ConfigNode(std::string value, std::unordered_map<std::string, ConfigNode> *sub_map) : is_copy_(false), value_(value), sub_map_(sub_map) {}
		/**
		 * @brief Construct a new empty ConfigNode object
		 * 
		 */
		ConfigNode(void) : is_copy_(false), value_(""), sub_map_(nullptr) {}
		/**
		 * @brief Copy construct a new ConfigNode object
		 * 
		 * Sets is_copy_ flag to prevent double deletion of *sub_map_
		 * 
		 * @param other ConfigNode to be copied
		 */
		ConfigNode(const ConfigNode &other)
			: is_copy_(true)
			, value_(other.value_)
			, sub_map_(other.sub_map_) {}
		/**
		 * @brief Move constructor
		 * 
		 * Move value_ and sub_map_ from other to this, and null out sub_map_ pointer in other
		 * 
		 * @param other ConfigNode to be moved
		 */
		ConfigNode(ConfigNode &&other)
			: is_copy_(false)
			, value_(std::move(other.value_))
			, sub_map_(std::move(other.sub_map_)) {
			other.sub_map_ = nullptr;
		}
		/**
		 * @brief Assignment move constructor
		 * 
		 * Move value_ and sub_map_ from other to this, and null out sub_map_ pointer in other
		 * 
		 * @param other ConfigNode to be moved
		 * @return ConfigNode& *this
		 */
		ConfigNode &operator=(ConfigNode &&other) {
			is_copy_ = false;
			value_ = std::move(other.value_);
			sub_map_ = std::move(other.sub_map_);
			other.sub_map_ = nullptr;
			return *this;
		}
		/**
		 * @brief Destroy the ConfigNode object
		 * 
		 * Deletes the sub_map_ member if allocated and not a copy
		 */
		~ConfigNode() {
			if (sub_map_ && !is_copy_)
				delete sub_map_;
		}
	};

	/**
	 * @brief Get config entry as type T from configuration map
	 * 
	 * @private
	 * 
	 * @tparam T Type of variable to return
	 * @param key Key to index map, from config file before '='
	 * @param config_map Map of keys to ConfigNode structs
	 * @return T Value returned from config
	 */
	template<class T>
	T get(const std::string &key, const std::unordered_map<std::string, ConfigNode> *config_map) {
		ConfigNode node = config_map->at(key);
		T result = boost::lexical_cast<T>(node.value_);
		return result;
	}

	/**
	 * @brief Main configuration parser class to inherit from in your code
	 * 
	 * Example usage:
	 * @include tests/simple/simple.cpp
	 * 
	 */
	class ConfigParser {
		/**
		 * @brief Guard to change config subsection within a scope:
		 * @ref ConfigSubsectionGuard
		 */
		friend class ConfigSubsectionGuard;
	private:
		/**
		 * @brief true if a ConfigSubsectionGuard is in scope
		 * Set in ConfigSubsectionGuard::ConfigSubsectionGuard()
		 * Cleared in ConfigSubsectionGuard::~ConfigSubsectionGuard()
		 */
		bool guarded_;
		std::unordered_map<std::string, ConfigNode> *config_map_ptr_; ///< Pointer to current config map
		std::string path_; ///< Path to config file
		std::unordered_map<std::string, ConfigNode> config_map_; ///< Map of config keys (std::string) to values (ConfigNode)
		/**
		 * @brief Iterate each line of config file and determine how to parse with l::check_record_type()
		 * 
		| Case | Function Called |
		| ---- | --------------- |
		| l::RecordType::UNK | prints error message and continues |
		| l::RecordType::ENTRY | ConfigParser::parse_entry() |
		| l::RecordType::HEADING | ConfigParser::parse_heading() |
		| l::RecordType::EMPTY | continues |
		 * 
		 * @param file Opened file stream for config file
		 */
		void parse(std::ifstream &file);
		/**
		 * @brief Extract value from config line and insert ConfigNode into config_map_
		 * 
		 * @param line String containing some form of "key = value"
		 */
		void parse_entry(const std::string &line);
		/**
		 * @brief Create new subconfig
		 * 
		 * construct new ConfigNode containing name and new config map,
		 * assign config_map_ptr_ to address of new config map,
		 * place into global config_map_,
		 * push address of new ConfigNode into sub_confs_
		 * 
		 * @param line String containing some form of "[Section Name]"
		 */
		void parse_heading(const std::string &line);
		/**
		 * @brief Update config_map_ptr_ to the subconfig map for section
		 * 
		 * @param section The config section to set to
		 */
		void set_subsection(const std::string &section) {
			config_map_ptr_ = config_map_.at(section).sub_map_;
			if(config_map_ptr_ == nullptr)
				throw std::out_of_range("ConfigNode has no sub_map_");
		}
		/**
		 * @brief Set config_map_ptr_ back to the address of config_map_
		 * 
		 */
		void reset_subsection(void) noexcept {
			config_map_ptr_ = &config_map_;
		}
	protected:
		std::vector<ConfigNode *> sub_confs_; ///< Vector of config subsections
	public:
		/**
		 * @brief Construct a new Config Parser object
		 * 
		 * Opens file at path as an std::ifstream and calls ConfigParser::parse()
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
		 * @brief Adapter for ffd::get(). This can throw. Use this in a try...catch block.
		 * 
		 * @code
		try {
			int integer = get<int>("Field");
		} catch (const ffd::ConfigException &err) {
			std::cerr << err.what();
		}
		 * @endcode
		 * @tparam T Type of variable to get
		 * @param key Key to index config_map_
		 * @return T Value returned from config
		 */
		template<class T>
		T get(const std::string &key) const {
			return ffd::get<T>(key, config_map_ptr_);
		}
		/**
		 * @brief Try to get value from config, default to fallback if fails. Guaranteed no-throw.
		 * 
		 * Example:
		 * @include tests/fallback/fallback.cpp
		 * 
		 * @tparam T Type of variable to return
		 * @param key Key to index config_map_
		 * @param fallback Default value to return if ffd::get() fails.
		 * @return T Value returned from config
		 */
		template<class T>
		T get(const std::string &key, const T &fallback) const noexcept {
			try {
				return get<T>(key);
			} catch (const std::out_of_range &) {
				// silently return fallback
			} catch (const boost::bad_lexical_cast &) {
				std::cerr << "Invalid configuration entry format: " << key << " = " << config_map_.at(key).value_ << std::endl;
			} catch (const ByteParseException &e) {
				std::cerr << e.what() << std::endl;
			} catch (const std::exception &e) {
				std::cerr << "Unexpected std::exception while getting " << key << ": " << e.what() << std::endl;
			} catch (const boost::exception &e) {
				std::cerr << "Unexpected boost::exception while getting " << key << ": " << boost::diagnostic_information(e) << std::endl;
			} catch ( ... ) {
				std::cerr << "Unexplained exception caught while getting " << key << std::endl;
			}
			return fallback;
		}
		/**
		 * @brief Try to get value from config. If ffd::get fails,
		 * return T() or 0 and set fail_flag. Guaranteed no-throw. Prints message
		 * to std::cerr to explain error.
		 * 
		 * Example:
		 * @include tests/simple/simple.cpp
		 * 
		 * @tparam T Type of variable to return
		 * @param key Key to index config_map_
		 * @param fail_flag Pointer to flag to set if ffd::get() fails
		 * @return T Value returned from config
		 */
		template<class T>
		T get(const std::string &key, bool *fail_flag) const noexcept {
			try {
				return get<T>(key);
			} catch (const boost::bad_lexical_cast &) {
				std::cerr << "Invalid configuration entry format: " << key << " = " << config_map_.at(key).value_ << std::endl;
			} catch (const std::out_of_range &) {
				std::cerr << "Option not in config: " << key << std::endl;
			} catch (const ByteParseException &e) {
				std::cerr << e.what() << std::endl;
			} catch (const std::exception &e) {
				std::cerr << "Unexpected std::exception while getting " << key << ": " << e.what() << std::endl;
			} catch (const boost::exception &e) {
				std::cerr << "Unexpected boost::exception while getting " << key << ": " << boost::diagnostic_information(e) << std::endl;
			} catch ( ... ) {
				std::cerr << "Unexplained exception caught while getting " << key << std::endl;
			}
			*fail_flag = true;
			if (std::is_fundamental<T>::value)
				return 0;
			else
				return T();
		}
		/**
		 * @brief Adapter for ffd::get(). Sets config_map_ptr_ to address of sub config with name section. This can throw.
		 * 
		 * @tparam T Type of variable to get
		 * @param section Subsection heading from config
		 * @param key Key to index config_map_
		 * @return T Value returned from config
		 */
		template<class T>
		T get_from(const std::string &section, const std::string &key) {
			if (guarded_)
				throw(ffd::ConfigGuardException("Cannot call get_from while ConfigSubsectionGuard is in scope"));
			set_subsection(section);
			return ffd::get<T>(key, config_map_ptr_);
			reset_subsection();
		}
		/**
		 * @brief Get value from config subsection using ConfigParser::get(const std::string&,const T&) const noexcept, return fallback if the subsection DNE
		 * 
		 * Example:
		 * @include tests/subsections/subsections.cpp
		 * 
		 * @tparam T Type to return
		 * @param section Config section name to get value from
		 * @param key Key identifying value in config
		 * @param fallback Returned if error or DNE
		 * @return T Value returned from config or fallback
		 */
		template<class T>
		T get_from(const std::string &section, const std::string &key, const T &fallback) noexcept {
			if (guarded_) {
				std::cerr << "Cannot call get_from while ConfigSubsectionGuard is in scope" << std::endl;
				return fallback;
			}
			try {
				set_subsection(section);
			} catch (const std::out_of_range &) {
				reset_subsection();
				return fallback;
			}
			return ffd::ConfigParser::get<T>(key, fallback);
			reset_subsection();
		}
		/**
		 * @brief Get value from config subsection using ConfigParser::get(const std::string&,bool*) const noexcept, set fail_flag if the subsection DNE
		 * 
		 * @tparam T Type to return
		 * @param section Config section name to get value from
		 * @param key Key identifying value in config
		 * @param fail_flag Pointer to flag to be set if error or DNE
		 * @return T Value returned from config or T() or 0
		 */
		template<class T>
		T get_from(const std::string &section, const std::string &key, bool *fail_flag) noexcept {
			if (guarded_) {
				std::cerr << "Cannot call get_from while ConfigSubsectionGuard is in scope" << std::endl;
				*fail_flag = true;
				if (std::is_fundamental<T>::value)
					return 0;
				else
					return T();
			}
			try {
				set_subsection(section);
			} catch (const std::out_of_range &) {
				std::cerr << "Section not in config." << std::endl;
				*fail_flag = true;
				reset_subsection();
				if (std::is_fundamental<T>::value)
					return 0;
				else
					return T();
			}
			return ffd::ConfigParser::get<T>(key, fail_flag);
			reset_subsection();
		}
	};

	/**
	 * @brief Use this to switch to a certain config subsection to get a group of values
	
	Inside ConfigParser method:
	\code
	{
		ConfigSubsectionGuard guard(*this, "Subsection 1");
		int section_1_value = get<int>("Value", -1);
		// section is switched back when guard goes out of scope
	}
	\endcode
	Any other scope:
	\code
	ConfigParser config("/etc/example.conf");
	{
		ConfigSubsectionGuard guard(config, "Subsection 1");
		int section_1_value = config.get<int>("Value", -1);
		// section is switched back when guard goes out of scope
	}
	\endcode
	Full Example:
	@include tests/dynamic_subsections/dynamic_subsections.cpp
	 */
	class ConfigSubsectionGuard {
	private:
		ConfigParser &config_; ///< Reference to ConfigParser or inhereting class
	public:
		/**
		 * @brief Construct a new Config Subsection Guard object
		 * 
		 * @param config ///< Reference to a ConfigParser or inhereting class
		 * @param section ///< Section name to switch to
		 */
		ConfigSubsectionGuard(ConfigParser &config, const std::string &section) : config_(config) {
			if (config_.guarded_)
				throw(ffd::ConfigGuardException("Tried to guard config when ConfigSubsectionGuard already in scope"));
			config_.guarded_ = true;
			config_.set_subsection(section);
		}
		/**
		 * @brief Destroy the Config Subsection Guard object
		 * 
		 */
		~ConfigSubsectionGuard(void) {
			config_.reset_subsection();
			config_.guarded_ = false;
		}
	};
}
