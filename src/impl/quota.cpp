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

#include "45d/Quota.hpp"
#include "45d/Bytes.hpp"
#include "45d/Exceptions.hpp"

#include <regex>

ffd::Quota::Quota(const ffd::Bytes &max, const std::string &str, ffd::Quota::RoundingMethod method)
	: ffd::Bytes(max), rounding_method_(method) {
    set_fraction(str);
}

double ffd::Quota::parse_fraction(const std::string &str) {
    double val;
	std::smatch m;
	if (regex_search(str, m, std::regex("^\\s*(\\d+\\.?\\d*)\\s*(%?)\\s*$"))) {
		try {
			val = std::stod(m.str(1));
		} catch (const std::invalid_argument &) {
			throw(ffd::QuotaParseException("Failed to interpret string as double: " + m.str(1)));
		}
		double divisor = (m.str(2).empty()) ? 1.0 : 100.0; // decimal or percent
		return val / divisor;
	} else {
		ffd::Bytes::bytes_type limit;
		try {
			limit = parse_bytes(str);
		} catch (const ffd::ByteParseException &e) {
			throw ffd::QuotaParseException(e.what());
		}
		return limit / bytes_;
	}
}
