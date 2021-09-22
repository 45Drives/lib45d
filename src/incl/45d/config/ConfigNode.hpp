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

#include <string>
#include <unordered_map>

namespace ffd {
	/**
	 * @brief Class for config_map_ entries
	 *
	 */
	class ConfigNode {
	public:
		std::string value_; ///< string from config file after '='
		std::unordered_map<std::string, ConfigNode>
			*sub_map_; ///< Pointer to submap for config sections
		/**
		 * @brief Construct a new ConfigNode object
		 *
		 * @param value String containing config value or subsection header
		 * @param sub_map nullptr or pointer to an std::unordered_map<std::string, ffd::ConfigNode>
		 * if subsection
		 */
		ConfigNode(std::string value, std::unordered_map<std::string, ConfigNode> *sub_map)
			: value_(value)
			, sub_map_(sub_map)
			, is_copy_(false) {}
		/**
		 * @brief Construct a new empty ConfigNode object
		 *
		 */
		ConfigNode(void) : value_(""), sub_map_(nullptr), is_copy_(false) {}
		/**
		 * @brief Copy construct a new ConfigNode object
		 *
		 * Sets is_copy_ flag to prevent double deletion of *sub_map_
		 *
		 * @param other ConfigNode to be copied
		 */
		ConfigNode(const ConfigNode &other)
			: value_(other.value_)
			, sub_map_(other.sub_map_)
			, is_copy_(true) {}
		/**
		 * @brief Move constructor
		 *
		 * Move value_ and sub_map_ from other to this, and null out sub_map_ pointer in other
		 *
		 * @param other ConfigNode to be moved
		 */
		ConfigNode(ConfigNode &&other)
			: value_(std::move(other.value_))
			, sub_map_(std::move(other.sub_map_))
			, is_copy_(false) {
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
			value_ = std::move(other.value_);
			sub_map_ = std::move(other.sub_map_);
			is_copy_ = false;
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
	private:
		bool is_copy_; ///< Set in copy constructor to avoid double deletion of *sub_map_
	};
} // namespace ffd
