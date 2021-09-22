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

#include <45d/config/ConfigParser.hpp>

namespace ffd {
	/**
	 * @brief Use this to switch to a certain config subsection to get a group of values

	Inside ConfigParser method:
	\code
	{
		ConfigSubsectionGuard guard(*this, "Subsection 1");
		int section_1_value = get<int>("Value", -1);
		// section is switched back when guard goes out of scope
	}
	\endcode
	Any other scope:
	\code
	ConfigParser config("/etc/example.conf");
	{
		ConfigSubsectionGuard guard(config, "Subsection 1");
		int section_1_value = config.get<int>("Value", -1);
		// section is switched back when guard goes out of scope
	}
	\endcode
	Full Example:
	@include tests/config/dynamic_subsections/dynamic_subsections.cpp
	 */
	class ConfigSubsectionGuard {
	public:
		/**
		 * @brief Construct a new Config Subsection Guard object
		 *
		 * @param config ///< Reference to a ConfigParser or inhereting class
		 * @param section ///< Section name to switch to
		 */
		ConfigSubsectionGuard(ConfigParser &config, const std::string &section) : config_(config) {
			if (config_.guarded_)
				throw(ffd::ConfigGuardException(
					"Tried to guard config when ConfigSubsectionGuard already in scope"));
			config_.guarded_ = true;
			config_.set_subsection(section);
		}
		/**
		 * @brief Destroy the Config Subsection Guard object
		 *
		 */
		~ConfigSubsectionGuard(void) {
			config_.reset_subsection();
			config_.guarded_ = false;
		}
	private:
		ConfigParser &config_; ///< Reference to ConfigParser or inhereting class
	};
} // namespace ffd
