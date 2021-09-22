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

extern "C" {
#include <sys/un.h>
}

namespace ffd {
	/**
	 * @brief Unix Socket Client class. Used for IPC through a named socket inode.
	 *
	 */
	class UnixSocketClient : public SocketBase {
	public:
		/**
		 * @brief Construct a new Unix Socket Client object
		 *
		 * @param path Path to socket inode
		 */
		UnixSocketClient(const std::string &path) : SocketBase(AF_UNIX, SOCK_STREAM) {
			memset(&server_addr_, 0, sizeof(server_addr_));
			server_addr_.sun_family = AF_UNIX;
			if (path.length() >= sizeof(server_addr_.sun_path))
				throw SocketAddressException("Socket address too long", ENAMETOOLONG);
			strncpy(server_addr_.sun_path, path.c_str(), sizeof(server_addr_.sun_path) - 1);
			io_fd_ = fd_;
		}
		/**
		 * @brief Make connection to socket.
		 *
		 * @return int fd for socket
		 */
		int connect() {
			int res = ::connect(fd_, (sockaddr *)&server_addr_, sizeof(server_addr_));
			if (res == -1) {
				int error = errno;
				throw SocketConnectException(strerror(error), error);
			}
			return io_fd_;
		}
	private:
		struct sockaddr_un server_addr_; ///< Unix socket address struct
	};
} // namespace ffd
