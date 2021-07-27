
#include "parser_functions.hpp"
#include <string>

void strip_whitespace(std::string &str) {
	size_t start, end;
	start = str.find_first_not_of(" \t");
	end = str.find_last_not_of(" \t");
	if (start == end) {
		str.clear();
		return;
	}
	str.assign(str, start, end - start + 1);
}

void remove_comments(std::string &str) {
	size_t comment_pos = str.find_first_of('#');
	if (comment_pos == std::string::npos)
		return;
	str.erase(comment_pos);
}

RecordType check_record_type(const std::string &record) {
	if (record[0] == '[' && record.find(']') != std::string::npos)
		return RecordType::HEADING;
	if (record.find('=') != std::string::npos)
		return RecordType::ENTRY;
	std::string::const_iterator itr = record.begin();
	while (itr != record.end() && (*itr == ' ' || *itr == '\t'))
		++itr;
	if (itr == record.end() || *itr == '#')
		return RecordType::EMPTY;
	return RecordType::UNK;
}
