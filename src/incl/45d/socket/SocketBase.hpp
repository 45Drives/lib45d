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

#include <45d/socket/Exceptions.hpp>
#include <sstream>
#include <vector>

extern "C" {
#include <string.h> // for strerror
#include <sys/socket.h>
#include <unistd.h> // for close
}

namespace ffd {
	/**
	 * @brief Default parameters for sockets
	 *
	 */
	namespace Socket {
		const int _backlog_default = 50; ///< Number of connections to queue
		/**
		 * @brief Size of recv buffer. Can be overridden by defining FFD_SOCKET_BUFF_SZ before
		 * including header.
		 *
		 */
		const int _buff_sz =
#ifndef FFD_SOCKET_BUFF_SZ
			1024;
#else
			FFD_SOCKET_BUFF_SZ;
#endif
		const char _rec_delim = 0x1E; ///< Record separator character
	}                                 // namespace Socket
	/**
	 * @brief Base Unix Socket Class for opening and closing the socket
	 *
	 */
	class SocketBase {
	public:
		/**
		 * @brief Construct a new Socket Base object.
		 * Opens a socket fd.
		 *
		 * see man socket(2) for details
		 *
		 * @param domain Usually AF_UNIX or AF_INET
		 * @param type Usually SOCK_STREAM
		 * @param protocol Normally just 0
		 */
		SocketBase(int domain, int type, int protocol = 0) : ACK('\6') {
			int res = socket(domain, type, protocol);
			if (res == -1) {
				int error = errno;
				throw SocketCreateException(strerror(error), error);
			}
			fd_ = res;
		}
		/**
		 * @brief Destroy the Socket Base object.
		 * Calls close() on the socket fd.
		 *
		 */
		~SocketBase() {
			close(fd_);
		}
		/**
		 * @brief Close a connection
		 *
		 * @param fd Socket connection fd or default for internal io_fd_
		 */
		void close_connection(int fd = 0) {
			if (fd == 0) {
				fd = io_fd_;
			}
			int res = close(fd);
			if (res == -1) {
				int error = errno;
				throw SocketCloseException(strerror(error), error);
			}
		}
		/**
		 * @brief Send a string
		 *
		 * @param str Message to send
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void send_data_async(const std::string &str, int flags = 0, int fd = 0) {
			if (fd == 0)
				fd = io_fd_;
			int res = send(fd, (void *)str.c_str(), str.length(), flags);
			if (res == -1) {
				int error = errno;
				throw SocketWriteException(strerror(error), error);
			}
		}
		/**
		 * @brief Send a string and wait for ACK
		 *
		 * @param str Message to send
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void send_data_sync(const std::string &str, int flags = 0, int fd = 0) {
			if (fd == 0)
				fd = io_fd_;
			send_data_async(str, flags, fd);
			get_ack(fd);
		}
		/**
		 * @brief Send a string and wait for ACK (alias for ffd::SocketBase::send_data_sync(const
		 * std::string&, int, int))
		 *
		 * @param str Message to send
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void send_data(const std::string &str, int flags = 0, int fd = 0) {
			send_data_sync(str, flags, fd);
		}
		/**
		 * @brief Send a vector as a record separator (0x1E) delimited string
		 *
		 * @param vec Vector to send
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void send_data_async(const std::vector<std::string> &vec, int flags = 0, int fd = 0) {
			std::string payload;
			std::vector<std::string>::const_iterator itr = vec.begin();
			payload = *itr;
			++itr;
			for (; itr != vec.end(); ++itr) {
				payload += Socket::_rec_delim + *itr;
			}
			send_data_async(payload, flags, fd);
		}
		/**
		 * @brief Send a vector as a record separator (0x1E) delimited string and wait for ACK
		 *
		 * @param vec Vector to send
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void send_data_sync(const std::vector<std::string> &vec, int flags = 0, int fd = 0) {
			send_data_async(vec, flags, fd);
			get_ack(fd);
		}
		/**
		 * @brief Send a vector as a record separator (0x1E) delimited string and wait for ACK
		 * (alias for ffd::SocketBase::send_data_sync(const std::vector<std::string>&, int, int))
		 *
		 * @param vec Vector to send
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void send_data(const std::vector<std::string> &vec, int flags = 0, int fd = 0) {
			send_data_sync(vec, fd, flags);
		}
		/**
		 * @brief Receive a string
		 *
		 * @param payload Received message returned by reference
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void receive_data_async(std::string &payload, int flags = 0, int fd = 0) {
			payload = "";
			if (fd == 0)
				fd = io_fd_;
			char buff[Socket::_buff_sz];
			memset(&buff, 0, sizeof(buff));
			int bytes_read = 0;
			do {
				bytes_read = recv(fd, &buff, sizeof(buff) - 1, flags);
				if (bytes_read == -1) {
					int error = errno;
					throw SocketReadException(strerror(error), error);
				}
				buff[bytes_read] = '\0';
				payload += buff;
			} while (bytes_read > 0
					 && recv(fd, &buff, sizeof(buff) - 1, MSG_PEEK | MSG_DONTWAIT) > 0);
		}
		/**
		 * @brief Receive a string and reply with ACK
		 *
		 * @param payload Received message returned by reference
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void receive_data_sync(std::string &payload, int flags = 0, int fd = 0) {
			receive_data_async(payload, flags, fd);
			send_ack(fd);
		}
		/**
		 * @brief Receive a string and reply with ACK (alias for
		 * ffd::SocketBase::receive_data_sync(std::string&, int, int))
		 *
		 * @param payload Received message returned by reference
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void receive_data(std::string &payload, int flags = 0, int fd = 0) {
			receive_data_sync(payload, flags, fd);
		}
		/**
		 * @brief Receive a vector as a record separator (0x1E) delimited string
		 *
		 * @param vec Received vector returned by reference
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void receive_data_async(std::vector<std::string> &vec, int flags = 0, int fd = 0) {
			vec.clear();
			std::string payload, record;
			receive_data_async(payload, flags, fd);
			std::stringstream ss(payload);
			while (std::getline(ss, record, Socket::_rec_delim)) {
				vec.push_back(record);
			}
		}
		/**
		 * @brief Receive a vector as a record separator (0x1E) delimited string and reply with ACK
		 *
		 * @param vec Received vector returned by reference
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void receive_data_sync(std::vector<std::string> &vec, int flags = 0, int fd = 0) {
			receive_data_async(vec, flags, fd);
			send_ack(fd);
		}
		/**
		 * @brief Receive a vector as a record separator (0x1E) delimited string and reply with ACK
		 * (alias for ffd::SocketBase::receive_data_sync(std::vector<std::string>&, int, int))
		 *
		 * @param vec Received vector returned by reference
		 * @param flags see man send(2)
		 * @param fd Optional file descriptor for connection
		 */
		void receive_data(std::vector<std::string> &vec, int flags = 0, int fd = 0) {
			receive_data_sync(vec, flags, fd);
		}
		/**
		 * @brief Call shutdown() on the socket fd, waking any blocked threads.
		 *
		 * @param how Either SHUT_RD, SHUT_WR, or SHUT_RDWR
		 */
		void shutdown(int how = SHUT_RDWR) {
			int res = ::shutdown(fd_, how);
			if (res == -1) {
				int error = errno;
				throw SocketShutdownException(strerror(error), error);
			}
		}
	protected:
		int fd_;    ///< File descriptor of socket
		int io_fd_; ///< Connection fd
		char ACK;   ///< char to send for acknowledging reception
	private:
		void get_ack(int fd) {
			if (fd == 0)
				fd = io_fd_;
			char ack_check;
			recv(fd, &ack_check, 1, 0);
			if (ack_check != ACK) {
				throw SocketWriteException("ACK failed");
			}
		}
		void send_ack(int fd) {
			if (fd == 0)
				fd = io_fd_;
			send(fd, &ACK, 1, 0);
		}
	};
} // namespace ffd
