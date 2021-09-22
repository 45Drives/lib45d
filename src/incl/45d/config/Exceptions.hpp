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

#include <45d/Exceptions.hpp>

namespace ffd {
	/**
	 * @brief General exception for all Config* related issues
	 *
	 */
	class ConfigException : public Exception {
	public:
		ConfigException(const std::string &what) noexcept : Exception(what) {}
	};

	/**
	 * @brief Throw this exception when the config file fails to open
	 *
	 */
	class NoConfigException : public ConfigException {
	public:
		NoConfigException(const std::string &what) noexcept : ConfigException(what) {}
	};

	/**
	 * @brief Throw this exception when a config entry is missing
	 *
	 */
	class MissingOptionException : public ConfigException {
	public:
		MissingOptionException(const std::string &what) noexcept : ConfigException(what) {}
	};

	/**
	 * @brief Throw this exception when a ConfigGuard is constructed
	 * or get_from() is called when the config is already guarded
	 *
	 */
	class ConfigGuardException : public ConfigException {
	public:
		ConfigGuardException(const std::string &what) noexcept : ConfigException(what) {}
	};
} // namespace ffd
