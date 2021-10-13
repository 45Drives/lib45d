#include <cassert>
#include <iostream>
#include <local/parser_functions.hpp>
#include <string>

std::string every_char_except_hash_and_ws =
	"!\"$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
std::string every_char_except_hash_and_ws_and_equals =
	"!\"$%&'()*+,-./0123456789:;<>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

void test_strip_whitespace() {
	std::cout << "testing l::strip_whitespace()" << std::endl;

	std::string test_string = " \t \t" + every_char_except_hash_and_ws + " \t \t";
	std::cout << "asserting whitespace is stripping from ends" << std::endl;
	l::strip_whitespace(test_string);
	assert(test_string == every_char_except_hash_and_ws);
	std::cout << "OK" << std::endl;

	// should strip whitespace only from ends of string
	test_string = " \t \t" + every_char_except_hash_and_ws + " \t \t"
				+ every_char_except_hash_and_ws + " \t \t";
	std::cout << "asserting whitespace only stripping from ends" << std::endl;
	l::strip_whitespace(test_string);
	assert(test_string == every_char_except_hash_and_ws + " \t \t" + every_char_except_hash_and_ws);
	std::cout << "OK" << std::endl;
}

void test_remove_comments() {
	std::cout << "testing l::remove_comments()" << std::endl;

	std::string test_string = every_char_except_hash_and_ws + "# \t" + every_char_except_hash_and_ws
							+ "# \t" + every_char_except_hash_and_ws;
	std::cout << "asserting everything after first '#' is removed" << std::endl;
	l::remove_comments(test_string);
	assert(test_string == every_char_except_hash_and_ws);
	std::cout << "OK";
}

void test_check_record_type() {
	std::cout << "testing l::check_record_type()" << std::endl;

	std::cout << "asserting empty string is l::RecordType::EMPTY" << std::endl;
	assert(l::check_record_type("") == l::RecordType::EMPTY);
	std::cout << "OK" << std::endl;

	std::cout << "asserting \"config key = value\" is l::RecordType::ENTRY" << std::endl;
	assert(l::check_record_type("config key = value") == l::RecordType::ENTRY);
	std::cout << "OK" << std::endl;

	std::cout << "asserting \"" + every_char_except_hash_and_ws + "\" is l::RecordType::ENTRY"
			  << std::endl;
	assert(l::check_record_type(every_char_except_hash_and_ws) == l::RecordType::ENTRY);
	std::cout << "OK" << std::endl;

	std::cout << "asserting \"key = \" is l::RecordType::ENTRY" << std::endl;
	assert(l::check_record_type("key = ") == l::RecordType::ENTRY);
	std::cout << "OK" << std::endl;

	std::cout << "asserting \" \\t[ \\t# \\t" + every_char_except_hash_and_ws
					 + " \\t] \\t\" is l::RecordType::HEADING"
			  << std::endl;
	assert(l::check_record_type(" \t[ \t# \t" + every_char_except_hash_and_ws + " \t] \t")
		   == l::RecordType::HEADING);
	std::cout << "OK" << std::endl;

	std::cout << "asserting \"" + every_char_except_hash_and_ws_and_equals
					 + "\" is l::RecordType::UNK"
			  << std::endl;
	assert(l::check_record_type(every_char_except_hash_and_ws_and_equals) == l::RecordType::UNK);
	std::cout << "OK" << std::endl;

	std::cout << "asserting \" = value\" is l::RecordType::UNK" << std::endl;
	assert(l::check_record_type(" = value") == l::RecordType::UNK);
	std::cout << "OK" << std::endl;
}

int main() {
	test_strip_whitespace();
	test_remove_comments();
	test_check_record_type();
	return 0;
}
