#include <45d/socket/UnixSocketClient.hpp>
#include <iostream>

#if __GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 32)
char *strerrorname_np(int err) {
	(void)err;
	return nullptr;
}
#endif

int main(void) {
	ffd::UnixSocketClient *client;

	try {
		client = new ffd::UnixSocketClient("test.socket");
	} catch (const ffd::SocketException &e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	try {
		client->connect();

		std::vector<std::string> send = {
			"Hello from client 1", "Hello from client 2", "Hello from client 3", "EOF"
		};
		client->send_data(send);

		std::vector<std::string> receive;
		client->receive_data(receive);
		for (const std::string &str : receive) {
			std::cout << str << std::endl;
		}

		client->close_connection();
	} catch (const ffd::SocketException &e) {
		std::cerr << e.what() << " " << strerrorname_np(e.get_errno()) << std::endl;
		client->close_connection();
		delete client;
		return -1;
	}
	delete client;
	return 0;
}
