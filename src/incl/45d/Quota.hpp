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
		 * @brief Rounding method to use when reporing bytes
		 * 
		 */
		enum RoundingMethod {NEAREST, DOWN, UP};
	private:
		double fraction_;
		RoundingMethod rounding_method_;
		intmax_t round(double x) const {
			switch (rounding_method_) {
				case RoundingMethod::NEAREST:
					return intmax_t(::round(x));
				case RoundingMethod::DOWN:
					return intmax_t(x);
				case RoundingMethod::UP:
					return intmax_t(ceil(x));
				default:
					return intmax_t(::round(x));
			}
		}
		void parse_fraction(const std::string &str);
	public:
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
			: Bytes(max), fraction_(fraction), rounding_method_(method) {}
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
			: Bytes(other.get()), fraction_(other.fraction_), rounding_method_(other.rounding_method_) {}
		/**
		 * @brief Move constructor
		 * 
		 * @param other Quota to be moved
		 */
		Quota(Quota &&other)
			: Bytes(std::move(other)), fraction_(std::move(other.fraction_)), rounding_method_(std::move(other.rounding_method_)) {}
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
		 * @brief Get value in bytes
		 * 
		 * @return intmax_t 
		 */
		intmax_t get(void) const {
			return round(double(bytes_) * fraction_);
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
	};
}