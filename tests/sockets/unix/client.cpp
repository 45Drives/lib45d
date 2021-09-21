#include <45d/socket/UnixSocketClient.hpp>
#include <iostream>

#if __GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 32)
char *strerrorname_np(int err) {
    (void) err;
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

        std::string messages[4] = {
            "Hello from client 1",
            "Hello from client 2",
            "Hello from client 3",
            "EOF"
        };
        for (int i = 0; i < 4; i++) {
            client->send_data(messages[i]);
        }

        std::string message;
        do {
            client->receive_data(message);
            std::cout << message << std::endl;
        } while (!message.empty() && message != "EOF");
        
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
