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

#include "45dconf.hpp"

#include <regex>

ffd::Bytes::Bytes(const std::string &str) {
	set(str);
}

void ffd::Bytes::set(const std::string &str) {
	double val;
	std::smatch m;
	if (!regex_search(str, m, std::regex("^(-?)\\s*(\\d+\\.?\\d*)\\s*([kKmMgGtTpPeEzZyY]?)(i?)[bB]\\s*$"))){
		throw(ffd::ByteParseException("Failed to parse string as bytes: " + str));
	}
	int sign = (m.str(1).empty()) ? 1 : -1;
	try {
		val = std::stod(m.str(2));
	} catch (const std::invalid_argument &) {
		throw(ffd::ByteParseException("Failed to interpret string as double: " + m.str(2)));
	}
	char prefix = (m.str(3).empty())? 0 : m.str(3).front();
	double base = (m.str(4).empty())? 1000.0 : 1024.0;
	double exp;
	switch(prefix){
		case 0:
			exp = 0.0;
			break;
		case 'k':
		case 'K':
			exp = 1.0;
			break;
		case 'm':
		case 'M':
			exp = 2.0;
			break;
		case 'g':
		case 'G':
			exp = 3.0;
			break;
		case 't':
		case 'T':
			exp = 4.0;
			break;
		case 'p':
		case 'P':
			exp = 5.0;
			break;
		case 'e':
		case 'E':
			exp = 6.0;
			break;
		case 'z':
		case 'Z':
			exp = 7.0;
			break;
		case 'y':
		case 'Y':
			exp = 8.0;
			break;
		default:
			throw(ffd::ByteParseException(std::string("Invalid unit prefix: ") + prefix + " (" + str + ")"));
	}
	bytes_ = sign * val * pow(base, exp);
}

std::string ffd::Bytes::get_str(ffd::Bytes::PrefixType prefix_type, int precision) const {
	const int N_PREFIXES = 9;
	const char prefixes[N_PREFIXES] = {'\0', 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y'};
	int sign = 1;
	uintmax_t bytes = get();
	if (bytes_ < 0) {
		sign = -1;
		bytes = -bytes;
	}
	std::stringstream formatted_ss;
	if(bytes_ == 0) return "0 B";
	double base = (prefix_type == ffd::Bytes::PrefixType::BINARY) ? 1024.0 : 1000;
	int prefix_ind = std::min(int(log(bytes) / log(base)), N_PREFIXES - 1);
	double p = pow(base, prefix_ind);
	double formatted = double(bytes) / p;
	if (sign == -1)
		formatted_ss << '-';
	if (prefixes[prefix_ind]) {
		formatted_ss << std::fixed << std::setprecision(precision) << formatted << " ";
		formatted_ss << prefixes[prefix_ind];
		if (prefix_type == ffd::Bytes::PrefixType::BINARY)
			formatted_ss << 'i';
	} else {
		formatted_ss << formatted << " ";
	}
	formatted_ss << 'B';
	return formatted_ss.str();
}