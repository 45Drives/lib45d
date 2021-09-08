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
#include <iostream>
#include <cmath>

namespace ffd {
    /**
	 * @brief Use this class for byte-formatted values. e.g.: "123 KiB"
	 * 
	 */
	class Bytes {
	protected:
		intmax_t bytes_;
	public:
		enum PrefixType {BINARY, SI};
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
		Bytes(intmax_t bytes) : bytes_(bytes) {}
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
		Bytes(const Bytes &other) : bytes_(other.bytes_) {}
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
		 * @return intmax_t 
		 */
		virtual intmax_t get(void) const {
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
		void set(intmax_t val) {
			bytes_ = val;
		}
		/**
		 * @brief Set value from formatted string
		 * 
		 * @param str 
		 */
		void set(const std::string &str);
		/**
		 * @brief Stream insertion operator
		 * 
		 * @param os 
		 * @param bytes 
		 * @return std::ostream& 
		 */
		friend std::ostream& operator<<(std::ostream& os, Bytes const &bytes) {
			return os << bytes.get_str();
		}
		/**
		 * @brief Stream extraction operator
		 * 
		 * @param is 
		 * @param bytes 
		 * @return std::istream& 
		 */
		friend std::istream& operator>>(std::istream& is, Bytes &bytes) {
			std::string str;
			std::getline(is, str);
			bytes.set(str);
			return is;
		}
		friend Bytes operator+(const Bytes &a, const Bytes &b) {
			return Bytes(a.get() + b.get());
		}
		friend Bytes operator-(const Bytes &a, const Bytes &b) {
			return Bytes(a.get() - b.get());
		}
		friend Bytes operator*(const Bytes &a, int &b) {
			return Bytes(a.get() * b);
		}
		friend Bytes operator*(int a, const Bytes &b) {
			return b * a;
		}
		friend Bytes operator/(const Bytes &a, int &b) {
			return Bytes(a.get() / b);
		}
	};
}