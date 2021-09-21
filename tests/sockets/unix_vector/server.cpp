#include <45d/socket/UnixSocketServer.hpp>
#include <iostream>
#include <vector>

#if __GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 32)
char *strerrorname_np(int err) {
    (void) err;
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

        std::vector<std::string> receive;
        server->receive_data(receive);
        for (const std::string &str : receive) {
            std::cout << str << std::endl;
        }

        std::vector<std::string> send = {
            "Hello from server 1",
            "Hello from server 2",
            "Hello from server 3",
            "EOF"
        };
        server->send_data(send);

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
