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

#include <45d/Bytes.hpp>

namespace ffd {
	/**
	 * @brief This class extends ffd::Bytes to specify percents of an amount of bytes.
	 *
	 */
	class Quota : public Bytes {
	public:
		/**
		 * @brief Rounding method enum
		 *
		 */
		enum RoundingMethod {
			NEAREST, ///< Round to nearest whole byte
			DOWN,    ///< Floor
			UP       ///< Ceiling
		};
		/**
		 * @brief Construct a new Quota object
		 *
		 * @param max Bytes to set maximum amount
		 * @param str String to be parsed into fraction
		 */
		Quota(const Bytes &max, const std::string &str, RoundingMethod method = NEAREST);
		/**
		 * @brief Construct a new Quota object
		 *
		 * @param max Bytes to set maximum amount
		 * @param fraction Fraction of max to represent
		 * @param method Rounding nearest, down, or up
		 */
		Quota(const Bytes &max, double fraction = 1.0, RoundingMethod method = NEAREST)
			: Bytes(max)
			, fraction_(fraction)
			, rounding_method_(method) {}
		Quota(const Bytes &max, const Bytes &allotted, RoundingMethod method = NEAREST)
			: Bytes(max)
			, fraction_(allotted / max)
			, rounding_method_(method) {}
		/**
		 * @brief Construct a new empty Quota object
		 *
		 */
		Quota(void) : Bytes(), fraction_(0), rounding_method_(NEAREST) {}
		/**
		 * @brief Copy constructor
		 *
		 * @param other
		 */
		Quota(const Quota &other)
			: Bytes(other.bytes_)
			, fraction_(other.fraction_)
			, rounding_method_(other.rounding_method_) {}
		/**
		 * @brief Move constructor
		 *
		 * @param other Quota to be moved
		 */
		Quota(Quota &&other)
			: Bytes(std::move(other))
			, fraction_(std::move(other.fraction_))
			, rounding_method_(std::move(other.rounding_method_)) {}
		/**
		 * @brief Copy assignment
		 *
		 * @param other Quota to be copied
		 * @return Quota& *this
		 */
		Quota &operator=(const Quota &other) {
			bytes_ = other.bytes_;
			fraction_ = other.fraction_;
			rounding_method_ = other.rounding_method_;
			return *this;
		}
		/**
		 * @brief Assignment move constructor
		 *
		 * @param other Quota to be moved
		 * @return Quota& *this
		 */
		Quota &operator=(Quota &&other) {
			bytes_ = std::move(other.bytes_);
			fraction_ = std::move(other.fraction_);
			rounding_method_ = std::move(other.rounding_method_);
			return *this;
		}
		/**
		 * @brief Destroy the Quota object
		 */
		~Quota() = default;
		void set_rounding_method(RoundingMethod method) {
			rounding_method_ = method;
		}
		/**
		 * @brief Get value in bytes equal to total bytes_ * fraction_
		 *
		 * @return bytes_type
		 */
		bytes_type get(void) const {
			return round(double(bytes_) * fraction_);
		}
		/**
		 * @brief Get the max number of bytes from parent
		 *
		 * @return bytes_type
		 */
		bytes_type get_max(void) const {
			return bytes_;
		}
		/**
		 * @brief Get just the fraction as a double
		 *
		 * @return double
		 */
		double get_fraction(void) const {
			return fraction_;
		}
		/**
		 * @brief Set the fraction
		 *
		 * @param fraction new value for fraction_
		 */
		void set_fraction(double fraction) {
			fraction_ = fraction;
		}
		/**
		 * @brief Set fraction from formatted string /\d+(.\d*)?\s*%?/
		 *
		 * @param str
		 */
		void set_fraction(const std::string &str) {
			fraction_ = parse_fraction(str);
		}
		/**
		 * @brief Parse str from a percentage to a decimal fraction
		 *
		 * @param str String to parse
		 * @return double Fraction to return
		 */
		double parse_fraction(const std::string &str);
	private:
		double fraction_; ///< The percent of bytes_ to report with get() and get_str()
		RoundingMethod rounding_method_; ///< Rounding method to use when reporing bytes
		/**
		 * @brief Round a double according to rounding_method_, called by get()
		 *
		 * @param x The number to round, most  likely double(bytes_) * fraction_
		 * @return bytes_type
		 */
		bytes_type round(double x) const {
			switch (rounding_method_) {
				case RoundingMethod::NEAREST:
					return bytes_type(::round(x));
				case RoundingMethod::DOWN:
					return bytes_type(x);
				case RoundingMethod::UP:
					return bytes_type(ceil(x));
				default:
					return bytes_type(::round(x));
			}
		}
	};
} // namespace ffd
