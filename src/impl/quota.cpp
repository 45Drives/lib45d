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

#include <45dconf.hpp>

#include <regex>

ffd::Quota::Quota(const ffd::Bytes &max, const std::string &str, ffd::Quota::RoundingMethod method)
	: ffd::Bytes(max), rounding_method_(method) {
    parse_fraction(str);
}

void ffd::Quota::parse_fraction(const std::string &str) {
    double val;
	std::smatch m;
	if (!regex_search(str, m, std::regex("^\\s*(\\d+\\.?\\d*)\\s*(%?)\\s*$"))){
		throw(ffd::QuotaParseException("Failed to parse string as bytes: " + str));
	}
	try {
		val = std::stod(m.str(1));
	} catch (const std::invalid_argument &) {
		throw(ffd::QuotaParseException("Failed to interpret string as double: " + m.str(1)));
	}
    double divisor = (m.str(2).empty()) ? 1.0 : 100.0; // decimal or percent
    fraction_ = val / divisor;
}
