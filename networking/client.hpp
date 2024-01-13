#ifndef CLIENT_HPP_INCLUDED
#define CLIENT_HPP_INCLUDED

#include <asio.hpp>
#include "messages.hpp"

using namespace asio::ip;

class AsyncClient {
public:
    AsyncClient(asio::io_context& io_context, const std::string& server_ip, unsigned short server_port)
        : io_context_(io_context),
          socket_(io_context),
          server_endpoint_(asio::ip::make_address(server_ip), server_port) {
    }

    void start() {
        // Start the asynchronous connection
        socket_.async_connect(server_endpoint_, [this](std::error_code ec) {
            if (!ec) {
                std::cout << "Connected to the server" << std::endl;

                // Start asynchronous read
                read();
                fire(123, 456, 789);
                // Start asynchronous user input
                //asyncUserInput();
            } else {
                std::cerr << "Connection error: " << ec.message() << std::endl;
            }
        });

        // Run the io_context to keep the program alive
        io_context_.run();
    }

    void fire(int x, int y, int player)
    {
        write(msg.strzal(x,y,player));
    }

private:

    void write(const std::string&  message) {
        // Data to be sent
        std::clog << "Writing: " << message << std::endl;

        // Start asynchronous write operation
        asio::async_write(socket_, asio::buffer(message),
                          [this, message](std::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                std::cout << "Sent: " << message << std::endl;

            } else {
                std::cerr << "Write error: " << ec.message() << std::endl;
            }
        });
    }

    void read() {
        // Start asynchronous read operation
        asio::async_read_until(socket_, inputBuffer_, '\n',
                                [this](std::error_code ec, std::size_t bytesRead) {
            if (!ec) {
                handleRead();
            } else {
                std::cerr << "Read error: " << ec.message() << std::endl;
            }
        });
    }

    void handleRead() {
        std::string message;
        std::istream is(&inputBuffer_);
        std::getline(is, message);
        std::clog << "Incoming message: " << message << std::endl;

        read();
    }


    asio::io_context& io_context_;
    tcp::socket socket_;
    tcp::endpoint server_endpoint_;
    //asio::streambuf userInputBuffer;
    asio::streambuf inputBuffer_;
    Messanger msg;
};

#endif // CLIENT_HPP_INCLUDED
