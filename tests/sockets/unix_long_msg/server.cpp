// very small buffer
#define SOCKET_BUFF_SZ 4

#include <45d/socket/UnixSocketServer.hpp>
#include <iostream>

#if __GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 32)
char *strerrorname_np(int err) {
	(void)err;
	return nullptr;
}
#endif

int main(void) {
	ffd::UnixSocketServer *server;

	try {
		server = new ffd::UnixSocketServer("test.socket");
	} catch (const ffd::SocketException &e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	try {
		server->wait_for_connection();

		std::string message;
		do {
			server->receive_data(message);
			std::cout << message << std::endl;
		} while (!message.empty() && message != "EOF");

		std::string messages[4] = {
			"Hello from server 1", "Hello from server 2", "Hello from server 3", "EOF"
		};
		for (int i = 0; i < 4; i++) {
			server->send_data(messages[i]);
		}

		server->close_connection();
	} catch (const ffd::SocketException &e) {
		std::cerr << e.what() << " " << strerrorname_np(e.get_errno()) << std::endl;
		server->close_connection();
		delete server;
		return -1;
	}
	delete server;
	return 0;
}
