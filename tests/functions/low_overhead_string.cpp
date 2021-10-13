#include <45d/low_overhead_string.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

void test_pattern_match_good_string(void) {
	const char *str = "45Drives";

	std::vector<std::string> good_patterns{ "45Drives", "45D*",     "45D**",    "*Drives",
											"**Drives", "45Dr*es",  "45Dr**es", "4?Dr?ves",
											"4*D*ri*s", "????????", "*" };

	std::vector<std::string> bad_patterns{
		"45Driv", "46D*", "*Dives", "45Dr?es", "",
	};

	for (const std::string &pattern : good_patterns) {
		std::cout << "Testing " << str << " == " << pattern << std::endl;
		assert(ffd::pattern_match(str, pattern.c_str()));
		std::cout << "OK" << std::endl;
	}

	for (const std::string &pattern : bad_patterns) {
		std::cout << "Testing " << str << " != " << pattern << std::endl;
		assert(!ffd::pattern_match(str, pattern.c_str()));
		std::cout << "OK" << std::endl;
	}
}

void test_pattern_match_empty_string(void) {
	const char *str = "";

	std::vector<std::pair<std::string, bool>> tests{ { "", true },       { "1", false },
													 { "*", true },      { "?", false },
													 { "a*b?c", false }, { "*abc*", false },
													 { "?abc?", false } };

	for (auto &test : tests) {
		std::cout << "Testing \"\" " << (test.second ? "== \"" : "!= \"") << test.first << '"'
				  << std::endl;
		assert(ffd::pattern_match(str, test.first.c_str()) == test.second);
		std::cout << "OK" << std::endl;
	}
}

int main(void) {
	test_pattern_match_good_string();
	test_pattern_match_empty_string();
	return 0;
}
