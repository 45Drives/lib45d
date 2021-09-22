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

#include <45d/socket/SocketBase.hpp>
#include <vector>

extern "C" {
#include <sys/un.h>
}

namespace ffd {
	/**
	 * @brief Unix Socket Server class. Used for IPC through a named socket inode.
	 *
	 */
	class UnixSocketServer : public SocketBase {
	public:
		/**
		 * @brief Construct a new Unix Socket Server object
		 *
		 * @param path Path to socket inode
		 * @param backlog Number of connections to queue
		 */
		UnixSocketServer(const std::string &path, int backlog = Socket::_backlog_default)
			: SocketBase(AF_UNIX, SOCK_STREAM)
			, domain_(AF_UNIX)
			, socket_path_(path) {
			bind();
			listen(backlog);
		}
		/**
		 * @brief Destroy the Unix Socket Server object
		 *
		 * Closes all connections and unlinks the socket inode
		 *
		 */
		~UnixSocketServer() {
			for (int connection : connections_) {
				close(connection);
			}
			unlink(socket_path_.c_str());
		}
		/**
		 * @brief Uses accept() to block and wait for a connection, returning a file descriptor
		 * to the connection. Also sets internal io_fd_ for later use.
		 *
		 * @return int fd of connection
		 */
		int wait_for_connection() {
			int connection_fd = accept(fd_, NULL, NULL);
			if (connection_fd == -1) {
				int error = errno;
				throw SocketAcceptException(strerror(error), error);
			}
			connections_.push_back(connection_fd);
			io_fd_ = connection_fd;
			return connection_fd;
		}
	private:
		const int domain_;              ///< Socket domain, always AF_UNIX
		const std::string socket_path_; ///< Path to socket inode, equal to address
		struct sockaddr_un sock_addr_;  ///< Unix socket address structs
		std::vector<int> connections_;  ///< vector of open connections for closing in dtor
		/**
		 * @brief Set up socket address structs and bind to the address inode path
		 *
		 */
		void bind(void) {
			memset(&sock_addr_, 0, sizeof(sock_addr_));
			sock_addr_.sun_family = domain_;
			if (socket_path_.length() >= sizeof(sock_addr_.sun_path))
				throw SocketAddressException("Socket address too long");
			strncpy(sock_addr_.sun_path, socket_path_.c_str(), sizeof(sock_addr_.sun_path) - 1);
			int res = ::bind(fd_, (sockaddr *)&sock_addr_, sizeof(sock_addr_));
			if (res == -1) {
				int error = errno;
				throw SocketBindException(strerror(error), error);
			}
		}
		/**
		 * @brief Start listening for connections
		 *
		 * @param backlog
		 */
		void listen(int backlog) {
			int res = ::listen(fd_, backlog);
			if (res == -1) {
				int error = errno;
				throw SocketListenException(strerror(error), error);
			}
		}
	};
} // namespace ffd
