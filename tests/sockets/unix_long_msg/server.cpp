#include <45d/socket/UnixSocketServer.hpp>
#include <iostream>

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
            message = server->receive_data();
            std::cout << message << std::endl;
        } while (!message.empty() && message != "EOF");

        std::string messages[4] = {
            "Hello from server 1",
            "Hello from server 2",
            "Hello from server 3",
            "EOF"
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
