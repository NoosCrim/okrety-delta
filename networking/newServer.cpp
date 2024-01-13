#include <iostream>
#include "server.hpp"


int main() {
    try {
        asio::io_context ioContext;
        Server server(ioContext, 12345);
        ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
