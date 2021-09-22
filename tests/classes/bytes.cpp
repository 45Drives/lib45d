#include <45d/Bytes.hpp>
#include <cassert>
#include <iostream>
#include <string>

void test_SI_to_binary(void) {
	std::string SI_in = "1000 KB";
	ffd::Bytes bytes(SI_in);
	std::string binary_out = bytes.get_str(ffd::Bytes::PrefixType::BINARY, 3);
	std::cout << "asserting " << SI_in << " == " << binary_out << std::endl;
	assert(binary_out == "976.562 KiB");
	std::cout << "OK" << std::endl;
}

void test_math(void) {
	ffd::Bytes a("100KiB");
	ffd::Bytes b(a);
	std::cout << "asserting " << a << " + " << b << " == 200 KiB" << std::endl;
	assert((a + b).get_str(ffd::Bytes::PrefixType::BINARY, 0) == "200 KiB");
	std::cout << "OK" << std::endl;

	std::cout << "asserting " << a << " * 2 == 200 KiB" << std::endl;
	assert((a * 2).get_str(ffd::Bytes::PrefixType::BINARY, 0) == "200 KiB");
	std::cout << "OK" << std::endl;

	std::cout << "asserting " << a << " * 2 == 2 * " << b << std::endl;
	assert((a * 2) == (2 * b));
	std::cout << "OK" << std::endl;

	std::cout << "asserting " << a << " / 2 == 50 KiB" << std::endl;
	assert((a / 2).get_str(ffd::Bytes::PrefixType::BINARY, 0) == "50 KiB");
	std::cout << "OK" << std::endl;

	std::cout << "asserting " << a << " - " << b << " / 2 == 50 KiB" << std::endl;
	assert((a - b / 2).get_str(ffd::Bytes::PrefixType::BINARY, 0) == "50 KiB");
	std::cout << "OK" << std::endl;
}

int main() {
	test_SI_to_binary();
	test_math();
	return 0;
}
