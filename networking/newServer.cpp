#include <iostream>
#include "server.hpp"



int main() {
    try {
        asio::io_context ioContext;
        Server server(ioContext, SERVER_PORT, STARTING_PLAYER_NUMBER);
        ioContext.dispatch();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
