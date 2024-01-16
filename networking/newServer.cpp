#include <iostream>
#include "server.hpp"

#define startingPlayerIndex 1


int main() {
    try {
        asio::io_context ioContext;
        Server server(ioContext, serverPORT, startingPlayerIndex);
        ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
