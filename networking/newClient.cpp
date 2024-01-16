#include <iostream>
#include "client.hpp"

int main() {
    try {
        // Replace "127.0.0.1" and 12345 with your server's IP address and port
        AsyncClient client("127.0.0.1", SERVER_PORT);
        if(client.start() == false)
        {
            std::clog << "Unsucessful connection" << std::endl;
        }
        else
        {
            std::clog << "Succsessful connection" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
