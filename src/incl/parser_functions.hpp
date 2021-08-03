/*
	Copyright (C) 2021 Joshua Boudreau <jboudreau@45drives.com>

	This file is part of lib45d_conf.

	lib45d_conf is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	lib45d_conf is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with lib45d_conf.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>

/**
 * @brief Local functions used in the compiled library
 * @private
 * 
 */
namespace l {
	/**
	* @brief Types of records in config file, returned by l::check_record_type()
	* @private
	*/
	enum class RecordType {
		UNK = -1, ///< unknown record type
		ENTRY, ///< "key = value" pair
		HEADING, ///< [string] style subsection header
		EMPTY ///< empty string
	};

	/**
	* @brief Modifies str by removing whitespace from beginning and end
	* 
	* @param str String to be modified
	* @private
	*/
	void strip_whitespace(std::string &str);

	/**
	* @brief Modifies str by removing anything after a '#' (inclusive)
	* 
	* @param str String to be modified
	* @private
	*/
	void remove_comments(std::string &str);

	/**
	* @brief Returns l::RecordType based on contents of record
	* 
	* @param record line of config file to check
	* @return RecordType 
	* @private
	*/
	RecordType check_record_type(const std::string &record);
};