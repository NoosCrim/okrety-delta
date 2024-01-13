#include <iostream>
#include "client.hpp"

int main() {
    try {
        asio::io_context io_context;

        // Replace "127.0.0.1" and 12345 with your server's IP address and port
        AsyncClient client(io_context, "127.0.0.1", 12345);

        // Start the asynchronous client
        client.start();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
