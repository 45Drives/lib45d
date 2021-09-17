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

extern "C" {
    #include <sys/socket.h>
    #include <string.h> // for strerror
    #include <unistd.h> // for close
}

namespace ffd {
    namespace Socket {
        const int _backlog_default = 50;
        const int _buff_sz = 1024;
    }
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
        SocketBase(int domain, int type, int protocol = 0) {
            int res = socket(domain, type, protocol);
            if (res == -1) {
                int error = errno;
                throw SocketCreateException(strerror(error));
            }
            fd_ = res;
        }
        /**
         * @brief Destroy the Socket Base object.
         * Calls close() on the socket fd.
         * 
         */
        ~SocketBase() {
            int res = close(fd_);
            if (res == -1) {
                int error = errno;
                throw SocketCloseException(strerror(error));
            }
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
                throw SocketCloseException(strerror(error));
            }
        }
        /**
         * @brief Send a string
         * 
         * @param str Message to send
         * @param fd Optional file descriptor for connection
         */
        void send_data(const std::string &str, int fd = 0) {
            if (fd == 0)
                fd = io_fd_;
            int res = write(io_fd_, (void *)str.c_str(), str.length()+1);
            if (res == -1) {
                int error = errno;
                throw SocketWriteException(strerror(error));
            }
        }
        /**
         * @brief Receive a string
         * 
         * @param fd Optional file descriptor for connection
         * @return std::string 
         */
        std::string receive_data(int fd = 0) {
            std::string result;
            if (fd == 0)
                fd = io_fd_;
            char buff[Socket::_buff_sz];
            memset(&buff, 0, sizeof(buff));
            int bytes_read = 0;
            do {
                bytes_read = read(fd, &buff, sizeof(buff) - 1);
                if (bytes_read > 0) {
                    buff[bytes_read] = '\0';
                    result += buff;
                }
            } while (bytes_read > 0);
            if (bytes_read == -1) {
                int error = errno;
                throw SocketReadException(strerror(error));
            }
            return result;
        }
    protected:
        int fd_; ///< File descriptor of socket
        int io_fd_; ///< Connection fd
    };
}
