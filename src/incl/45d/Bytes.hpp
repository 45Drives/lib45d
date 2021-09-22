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

#include <cmath>
#include <iostream>
#include <string>

namespace ffd {
	/**
	 * @brief Use this class for byte-formatted values. e.g.: "123 KiB"
	 *
	 */
	class Bytes {
	public:
		typedef off64_t bytes_type;     ///< Type to store number of bytes
		enum PrefixType { BINARY, SI }; ///< BINARY for multiples of 1024, SI for multiples of 1000
		/**
		 * @brief Construct a new Bytes object from formatted string
		 *
		 * @param str formatted string for bytes
		 */
		Bytes(const std::string &str);
		/**
		 * @brief Construct a new Bytes object from integral type
		 *
		 * @param bytes
		 */
		Bytes(bytes_type bytes) : bytes_(bytes) {}
		/**
		 * @brief Construct a new empty Bytes object
		 *
		 */
		Bytes(void) : bytes_(0) {}
		/**
		 * @brief Copy construct a new Bytes object
		 *
		 * @param other Bytes to be copied
		 */
		Bytes(const Bytes &other) : bytes_(other.get()) {}
		/**
		 * @brief Move constructor
		 *
		 * @param other Bytes to be moved
		 */
		Bytes(Bytes &&other) : bytes_(std::move(other.bytes_)) {}
		/**
		 * @brief Copy assignment
		 *
		 * @param other Bytes to be copied
		 * @return Bytes& *this
		 */
		Bytes &operator=(const Bytes &other) {
			bytes_ = other.get();
			return *this;
		}
		/**
		 * @brief Assignment move constructor
		 *
		 * @param other Bytes to be moved
		 * @return Bytes& *this
		 */
		Bytes &operator=(Bytes &&other) {
			bytes_ = std::move(other.bytes_);
			return *this;
		}
		/**
		 * @brief Destroy the Bytes object
		 */
		~Bytes() = default;
		/**
		 * @brief Get value in bytes
		 *
		 * @return bytes_type
		 */
		virtual bytes_type get(void) const {
			return bytes_;
		}
		/**
		 * @brief Get value as formatted string
		 *
		 * @param prefix_type ffd::Bytes::PrefixType::BINARY for multiples of 1024,
		 * ffd::Bytes::PrefixType::SI for multiples of 1000.
		 *
		 * @return std::string
		 */
		std::string get_str(enum PrefixType prefix_type = BINARY, int precision = 2) const;
		/**
		 * @brief Set value from integral type
		 *
		 * @param val
		 */
		void set(bytes_type val) {
			bytes_ = val;
		}
		/**
		 * @brief Set value from formatted string /\d+(.\d*)?\s*[kmgtpezy]?i?b/i
		 *
		 * @param str
		 */
		void set(const std::string &str) {
			bytes_ = parse_bytes(str);
		}
		/**
		 * @brief Stream insertion operator
		 *
		 * @param os
		 * @param bytes
		 * @return std::ostream&
		 */
		friend std::ostream &operator<<(std::ostream &os, Bytes const &bytes) {
			return os << bytes.get_str();
		}
		/**
		 * @brief Stream extraction operator
		 *
		 * @param is
		 * @param bytes
		 * @return std::istream&
		 */
		friend std::istream &operator>>(std::istream &is, Bytes &bytes) {
			std::string str;
			std::getline(is, str);
			bytes.set(str);
			return is;
		}
		Bytes &operator+=(const Bytes &other) {
			bytes_ += other.get();
			return *this;
		}
		Bytes &operator-=(const Bytes &other) {
			bytes_ -= other.get();
			return *this;
		}
		friend Bytes operator+(const Bytes &a, const Bytes &b) {
			return Bytes(a.get() + b.get());
		}
		friend Bytes operator-(const Bytes &a, const Bytes &b) {
			return Bytes(a.get() - b.get());
		}
		friend Bytes operator*(const Bytes &a, int b) {
			return Bytes(a.get() * b);
		}
		friend Bytes operator*(int a, const Bytes &b) {
			return b * a;
		}
		friend Bytes operator/(const Bytes &a, int b) {
			return Bytes(a.get() / b);
		}
		friend double operator/(const Bytes &a, const Bytes &b) {
			return double(a.get()) / double(b.get());
		}
		friend bool operator==(const Bytes &a, const Bytes &b) {
			return a.get() == b.get();
		}
		friend bool operator!=(const Bytes &a, const Bytes &b) {
			return !(operator==(a, b));
		}
		friend bool operator<(const Bytes &a, const Bytes &b) {
			return a.get() < b.get();
		}
		friend bool operator>(const Bytes &a, const Bytes &b) {
			return b.get() < a.get();
		}
		friend bool operator<=(const Bytes &a, const Bytes &b) {
			return !(operator>(a, b));
		}
		friend bool operator>=(const Bytes &a, const Bytes &b) {
			return !(operator<(a, b));
		}
	protected:
		bytes_type bytes_; ///< The actual value
		/**
		 * @brief Parse bytes from string
		 *
		 * @param str String to parse
		 * @return bytes_type Bytes to return
		 */
		bytes_type parse_bytes(const std::string &str) const;
	};
} // namespace ffd
