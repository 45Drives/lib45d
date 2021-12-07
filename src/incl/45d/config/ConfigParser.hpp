// -*- C++ -*-
/*
 *    Copyright (C) 2021 Joshua Boudreau <jboudreau@45drives.com>
 *
 *    This file is part of lib45d.
 *
 *    lib45d is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    lib45d is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with lib45d.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <45d/Quota.hpp>
#include <45d/config/ConfigNode.hpp>
#include <45d/config/Exceptions.hpp>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Namespace for internal use, not to be exposed to ffd
 *
 * @private
 *
 */
namespace ffd_internal {
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
	T get(const std::string &key,
		  const std::unordered_map<std::string, ffd::ConfigNode> *config_map) {
		ffd::ConfigNode node = config_map->at(key);
		std::stringstream ss(node.value_);
		ss.exceptions(std::ios::failbit | std::ios::badbit);
		T result;
		ss >> result;
		return result;
	}
} // namespace ffd_internal

namespace ffd {
	/**
	 * @brief Main configuration parser class to inherit from in your code
	 *
	 * Example usage:
	 * @include tests/config/simple/simple.cpp
	 *
	 */
	class ConfigParser {
		/**
		 * @brief Guard to change config subsection within a scope:
		 * @ref ConfigSubsectionGuard
		 */
		friend class ConfigSubsectionGuard;
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
		 * @brief Get value from config map using ffd::get(). This can throw. Use this in a
		try...catch block.
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
			return ffd_internal::get<T>(key, config_map_ptr_);
		}
		/**
		 * @brief Try to get value from config, default to fallback if fails. Guaranteed no-throw.
		 *
		 * Example:
		 * @include tests/config/fallback/fallback.cpp
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
			} catch (const std::ios_base::failure &) {
				try {
					report_error("Invalid configuration entry format: " + key + " = "
							 + config_map_.at(key).value_);
				} catch (const std::out_of_range &) {
					report_error("Invalid configuration entry format for " + key);
				}
			} catch (const ConfigException &e) {
				report_error(e.what());
			} catch (const std::exception &e) {
				report_error("Unexpected std::exception while getting " + key + ": " + e.what());
			} catch (...) {
				report_error("Unexplained exception caught while getting " + key);
			}
			return fallback;
		}
		/**
		 * @brief Try to get value from config. If ffd::get fails,
		 * return T() or 0 and set fail_flag. Guaranteed no-throw. Prints message
		 * to std::cerr to explain error.
		 *
		 * Example:
		 * @include tests/config/simple/simple.cpp
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
			} catch (const std::ios_base::failure &) {
				try {
					report_error("Invalid configuration entry format: " + key + " = "
							 + config_map_.at(key).value_);
				} catch (const std::out_of_range &) {
					report_error("Invalid configuration entry format for " + key);
				}
			} catch (const std::out_of_range &) {
				report_error("Option not in config: " + key);
			} catch (const ConfigException &e) {
				report_error(e.what());
			} catch (const std::exception &e) {
				report_error("Unexpected std::exception while getting " + key + ": " + e.what());
			} catch (...) {
				report_error("Unexplained exception caught while getting " + key);
			}
			*fail_flag = true;
			if (std::is_fundamental<T>::value)
				return 0;
			else
				return T();
		}
		/**
		 * @brief Adapter for ffd::get(). Sets config_map_ptr_ to address of sub config with name
		 * section. This can throw.
		 *
		 * @tparam T Type of variable to get
		 * @param section Subsection heading from config
		 * @param key Key to index config_map_
		 * @return T Value returned from config
		 */
		template<class T>
		T get_from(const std::string &section, const std::string &key) {
			if (guarded_)
				throw(ffd::ConfigGuardException(
					"Cannot call get_from while ConfigSubsectionGuard is in scope"));
			set_subsection(section);
			T result = get<T>(key);
			reset_subsection();
			return result;
		}
		/**
		 * @brief Get value from config subsection using ConfigParser::get(const std::string&,const
		 * T&) const noexcept, return fallback if the subsection DNE
		 *
		 * Example:
		 * @include tests/config/subsections/subsections.cpp
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
				std::cerr << "Cannot call get_from while ConfigSubsectionGuard is in scope"
						  << std::endl;
				return fallback;
			}
			try {
				set_subsection(section);
			} catch (const std::out_of_range &) {
				reset_subsection();
				return fallback;
			}
			T result = get<T>(key, fallback);
			reset_subsection();
			return result;
		}
		/**
		 * @brief Get value from config subsection using ConfigParser::get(const std::string&,bool*)
		 * const noexcept, set fail_flag if the subsection DNE
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
				std::cerr << "Cannot call get_from while ConfigSubsectionGuard is in scope"
						  << std::endl;
				*fail_flag = true;
				if (std::is_fundamental<T>::value)
					return 0;
				else
					return T();
			}
			try {
				set_subsection(section);
			} catch (const std::out_of_range &) {
				std::cerr << "Section not in config: " << section << std::endl;
				*fail_flag = true;
				reset_subsection();
				if (std::is_fundamental<T>::value)
					return 0;
				else
					return T();
			}
			T result = get<T>(key, fail_flag);
			reset_subsection();
			return result;
		}
		/**
		 * @brief Get quota from config map using ffd::get(). This can throw. Use this in a
		try...catch block.
		 *
		 * @code
		ffd::Bytes total(1024);
		try {
			ffd::Quota quota = get("Quota", total);
		} catch (const ffd::ConfigException &err) {
			std::cerr << err.what();
		}
		 * @endcode
		 * @param key Key to index config_map_
		 * @param max Maximum number of bytes
		 * @return ffd::Quota Value returned from config
		 */
		Quota get_quota(const std::string &key, const Bytes &max) const {
			return Quota(max, get<std::string>(key));
		}
		/**
		 * @brief Try to get Quota from config, default to fallback if fails. Guaranteed no-throw.
		 *
		 * @param key Key to index config_map_
		 * @param max Maximum number of bytes
		 * @param fallback Default value to return if ffd::get() fails.
		 * @return T Value returned from config
		 */
		Quota
		get_quota(const std::string &key, const Bytes &max, const Quota &fallback) const noexcept {
			try {
				return get_quota(key, max);
			} catch (const std::out_of_range &) {
				// silently return fallback
			} catch (const std::ios_base::failure &) {
				try {
					report_error("Invalid configuration entry format: " + key + " = "
							 + config_map_.at(key).value_);
				} catch (const std::out_of_range &) {
					report_error("Invalid configuration entry format for " + key);
				}
			} catch (const ConfigException &e) {
				report_error(e.what());
			} catch (const std::exception &e) {
				report_error("Unexpected std::exception while getting " + key + ": " + e.what());
			} catch (...) {
				report_error("Unexplained exception caught while getting " + key);
			}
			return fallback;
		}
		/**
		 * @brief Try to get Quota from config. If ffd::get fails,
		 * return Quota(void) and set fail_flag. Guaranteed no-throw. Prints message
		 * to std::cerr to explain error.
		 *
		 * @param key Key to index config_map_
		 * @param max Maximum number of bytes
		 * @param fail_flag Pointer to flag to set if ffd::get() fails
		 * @return T Value returned from config
		 */
		Quota get_quota(const std::string &key, const Bytes &max, bool *fail_flag) const noexcept {
			try {
				return get_quota(key, max);
			} catch (const std::ios_base::failure &) {
				try {
					report_error("Invalid configuration entry format: " + key + " = "
							 + config_map_.at(key).value_);
				} catch (const std::out_of_range &) {
					report_error("Invalid configuration entry format for " + key);
				}
			} catch (const std::out_of_range &) {
				report_error("Option not in config: " + key);
			} catch (const ConfigException &e) {
				report_error(e.what());
			} catch (const std::exception &e) {
				report_error("Unexpected std::exception while getting " + key + ": " + e.what());
			} catch (...) {
				report_error("Unexplained exception caught while getting " + key);
			}
			*fail_flag = true;
			return Quota();
		}
		/**
		 * @brief Adapter for ffd::get(). Sets config_map_ptr_ to address of sub config with name
		 * section. This can throw.
		 *
		 * @param section Subsection heading from config
		 * @param key Key to index config_map_
		 * @param max Maximum number of bytes
		 * @return Quota Value returned from config
		 */
		Quota get_quota_from(const std::string &section, const std::string &key, const Bytes &max) {
			if (guarded_)
				throw(ffd::ConfigGuardException(
					"Cannot call get_from while ConfigSubsectionGuard is in scope"));
			set_subsection(section);
			Quota result = get_quota(key, max);
			reset_subsection();
			return result;
		}
		/**
		 * @brief Get value from config subsection using ConfigParser::get(const std::string&,const
		 * T&) const noexcept, return fallback if the subsection DNE
		 *
		 * Example:
		 * @include tests/config/subsections/subsections.cpp
		 *
		 * @param section Config section name to get value from
		 * @param key Key identifying value in config
		 * @param max Maximum number of bytes
		 * @param fallback Returned if error or DNE
		 * @return Quota Value returned from config or fallback
		 */
		Quota get_quota_from(const std::string &section,
							 const std::string &key,
							 const Bytes &max,
							 const Quota &fallback) noexcept {
			if (guarded_) {
				std::cerr << "Cannot call get_from while ConfigSubsectionGuard is in scope"
						  << std::endl;
				return fallback;
			}
			try {
				set_subsection(section);
			} catch (const std::out_of_range &) {
				reset_subsection();
				return fallback;
			}
			Quota result = get_quota(key, max, fallback);
			reset_subsection();
			return result;
		}
		/**
		 * @brief Get value from config subsection using ConfigParser::get(const std::string&,bool*)
		 * const noexcept, set fail_flag if the subsection DNE
		 *
		 * @param section Config section name to get value from
		 * @param key Key identifying value in config
		 * @param max Maximum number of bytes
		 * @param fail_flag Pointer to flag to be set if error or DNE
		 * @return Quota Value returned from config or T() or 0
		 */
		Quota get_quota_from(const std::string &section,
							 const std::string &key,
							 const Bytes &max,
							 bool *fail_flag) noexcept {
			if (guarded_) {
				std::cerr << "Cannot call get_from while ConfigSubsectionGuard is in scope"
						  << std::endl;
				*fail_flag = true;
				return Quota();
			}
			try {
				set_subsection(section);
			} catch (const std::out_of_range &) {
				std::cerr << "Section not in config." << std::endl;
				*fail_flag = true;
				reset_subsection();
				return Quota();
			}
			Quota result = get_quota(key, max, fail_flag);
			reset_subsection();
			return result;
		}
	protected:
		std::vector<ConfigNode *> sub_confs_; ///< Vector of config subsections
		std::string current_section_;         ///< Name of current section, set by set_subsection()
	private:
		/**
		 * @brief true if a ConfigSubsectionGuard is in scope
		 * Set in ConfigSubsectionGuard::ConfigSubsectionGuard()
		 * Cleared in ConfigSubsectionGuard::~ConfigSubsectionGuard()
		 */
		bool guarded_;
		std::unordered_map<std::string, ConfigNode>
			*config_map_ptr_; ///< Pointer to current config map
		std::string path_;    ///< Path to config file
		std::unordered_map<std::string, ConfigNode>
			config_map_; ///< Map of config keys (std::string) to values (ConfigNode)
		/**
		 * @brief Iterate each line of config file and determine how to parse with
		l::check_record_type()
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
			try {
				config_map_ptr_ = config_map_.at(section).sub_map_;
			} catch (const std::out_of_range &) {
				throw std::out_of_range("No subconfig with name `" + section + "`");
			}
			if (config_map_ptr_ == nullptr)
				throw std::out_of_range("`" + section + "` is not a subsection.");
			current_section_ = section;
		}
		/**
		 * @brief Set config_map_ptr_ back to the address of config_map_
		 *
		 */
		void reset_subsection(void) noexcept {
			config_map_ptr_ = &config_map_;
			current_section_ = "";
		}
		/**
		 * @brief Print error message to stderr, conditionally
		 * prepended with current subsection name
		 *
		 * @param message
		 */
		void report_error(const std::string &message) const noexcept {
			if (config_map_ptr_ != &config_map_)
				std::cerr << "[" << current_section_ << "]: ";
			std::cerr << message << std::endl;
		}
	};
} // namespace ffd
