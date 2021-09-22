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

#include <string>

namespace ffd {
	/**
	 * @brief Exceptions thrown by this library
	 *
	 */
	class Exception {
	public:
		/**
		 * @brief Construct a new Exception object
		 *
		 * @param what String containing explanation message
		 * @param err Optionally pass errno
		 */
		Exception(const std::string &what, int err = 0) noexcept : what_(what), errno_(err) {}
		/**
		 * @brief Return string containing explanation message
		 *
		 * @return std::string
		 */
		const char *what(void) const noexcept {
			return what_.c_str();
		}
		/**
		 * @brief Get the errno relating to the error, 0 if not set.
		 *
		 * @return int
		 */
		int get_errno(void) const noexcept {
			return errno_;
		}
	private:
		std::string what_; ///< String containing explanation message
		int errno_;        ///< Optionally save errno if applicable
	};

	/**
	 * @brief Throw this exception when Bytes::set() fails to parse string
	 *
	 */
	class ByteParseException : public Exception {
	private:
		std::string what_; ///< String containing explanation message
	public:
		ByteParseException(const std::string &what) noexcept : Exception(what) {}
	};

	/**
	 * @brief Throw this exception when Quota::parse_fraction() fails to parse string
	 *
	 */
	class QuotaParseException : public Exception {
	private:
		std::string what_; ///< String containing explanation message
	public:
		QuotaParseException(const std::string &what) noexcept : Exception(what) {}
	};
} // namespace ffd
