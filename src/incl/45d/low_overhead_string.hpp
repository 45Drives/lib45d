// -*- C++ -*-
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

#pragma once

namespace ffd {
	/**
	 * @brief Match a string against a wildcard pattern with '?' meaning any character and '*'
	 * meaning zero or more characters
	 *
	 * @param str String to check (nul terminated)
	 * @param pattern Pattern to check against (nul terminated)
	 * @return true String matches pattern
	 * @return false String does not match pattern
	 */
	inline bool pattern_match(const char *const str, const char *const pattern) {
		const char *pattern_itr = pattern;
		const char *str_itr = str;
		const char *wildcard_pat = nullptr;
		const char *wildcard_str = nullptr;
		while (*pattern_itr != '\0' && *str_itr != '\0') {
			if (*pattern_itr == '?' || *pattern_itr == *str_itr) {
				str_itr++;
				pattern_itr++;
			} else if (*pattern_itr == '*') {
				wildcard_str = str_itr;
				wildcard_pat = pattern_itr;
				pattern_itr++;
			} else if (wildcard_pat) {
				pattern_itr = wildcard_pat + 1;
				str_itr = wildcard_str + 1;
				wildcard_str++;
			} else {
				return false;
			}
		}

		if (pattern_itr > pattern && *(pattern_itr - 1) == '*')
			while (*str_itr != '\0')
				str_itr++;

		while (*pattern_itr == '*')
			pattern_itr++;

		return *pattern_itr == '\0' && *str_itr == '\0';
	}
} // namespace ffd
