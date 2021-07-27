#pragma once

#include <string>

/// Types of records in config file, returned by check_record_type()
enum class RecordType {
	UNK = -1, /// unknown record type
	ENTRY, /// "key = value" pair
	HEADING, /// [string] style subsection header
	EMPTY /// empty string
};

/**
 * @brief Modifies str by removing whitespace from beginning and end
 * 
 * @param str String to be modified
 */
void strip_whitespace(std::string &str);

/**
 * @brief Modifies str by removing anything after a '#'
 * 
 * @param str String to be modified
 */
void remove_comments(std::string &str);

/**
 * @brief Returns RecordType based on contents of record
 * 
 * @param record line of config file to check
 * @return RecordType 
 */
RecordType check_record_type(const std::string &record);
