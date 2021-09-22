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
	 * @brief General exception for all Socket* related issues
	 *
	 */
	class SocketException : public Exception {
	public:
		SocketException(const std::string &what, int err = 0) : Exception(what, err) {}
	};

	/**
	 * @brief Thrown when socket() returns -1 with strerror(errno) as what
	 *
	 */
	class SocketCreateException : public SocketException {
	public:
		SocketCreateException(const std::string &what, int err = 0) : SocketException(what, err) {}
	};

	/**
	 * @brief Thrown when close(fd_) returns -1 with strerror(errno) as what
	 *
	 */
	class SocketCloseException : public SocketException {
	public:
		SocketCloseException(const std::string &what, int err = 0) : SocketException(what, err) {}
	};

	/**
	 * @brief Thrown when the socket address is too long
	 *
	 */
	class SocketAddressException : public SocketException {
	public:
		SocketAddressException(const std::string &what, int err = 0) : SocketException(what, err) {}
	};

	/**
	 * @brief Thrown when bind() fails
	 *
	 */
	class SocketBindException : public SocketException {
	public:
		SocketBindException(const std::string &what, int err = 0) : SocketException(what, err) {}
	};

	class SocketListenException : public SocketException {
	public:
		SocketListenException(const std::string &what, int err = 0) : SocketException(what, err) {}
	};

	/**
	 * @brief Thrown when accept() fails
	 *
	 */
	class SocketAcceptException : public SocketException {
	public:
		SocketAcceptException(const std::string &what, int err = 0) : SocketException(what, err) {}
	};

	/**
	 * @brief Thrown when connect() fails
	 *
	 */
	class SocketConnectException : public SocketException {
	public:
		SocketConnectException(const std::string &what, int err = 0) : SocketException(what, err) {}
	};

	/**
	 * @brief Thrown when write() fails
	 *
	 */
	class SocketWriteException : public SocketException {
	public:
		SocketWriteException(const std::string &what, int err = 0) : SocketException(what, err) {}
	};

	/**
	 * @brief Thrown when read() fails
	 *
	 */
	class SocketReadException : public SocketException {
	public:
		SocketReadException(const std::string &what, int err = 0) : SocketException(what, err) {}
	};

	/**
	 * @brief Thrown when shutdown() fails
	 *
	 */
	class SocketShutdownException : public SocketException {
	public:
		SocketShutdownException(const std::string &what, int err = 0)
			: SocketException(what, err) {}
	};
} // namespace ffd
