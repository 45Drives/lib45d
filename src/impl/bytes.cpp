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
	if (!regex_search(str, m, std::regex("^(\\d+\\.?\\d*)\\s*([kKmMgGtTpPeEzZyY]?)(i?)[bB]$"))){
		throw(ffd::ByteParseException("Failed to parse string as bytes: " + str));
	}
	try {
		val = std::stod(m[1]);
	} catch (const std::invalid_argument &) {
		throw(ffd::ByteParseException("Failed to interpret string as double: " + m.str(1)));
	}
	char prefix = (m.str(2).empty())? 0 : m.str(2).front();
	double base = (m.str(3).empty())? 1000.0 : 1024.0;
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
	bytes_ = val * pow(base, exp);
}