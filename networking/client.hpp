#ifndef CLIENT_HPP_INCLUDED
#define CLIENT_HPP_INCLUDED

#include <asio.hpp>
#include <functional>
#include "messages.hpp"

using namespace Messanger;
using namespace asio::ip;

class AsyncClient {
public:
    AsyncClient(const std::string& server_ip, unsigned short server_port)
        : socket_(io_context_),
          server_endpoint_(asio::ip::make_address(server_ip), server_port) {
              start();
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
        write(strzal(x,y,player));
    }

    void trafil(int x, int y, int player)
    {
        write(trafiony(x,y,player));
    }

    void nieTrafil(int x, int y, int player)
    {
        write(nieTrafiony(x,y,player));
    }

    void niePoprawnieStrzelil(int x, int y, int player)
    {
        write(niePoprawnyStrzal(x,y,player));
    }

    void graczPrzegral(int player)
    {
        write(przegranaGracza(player));
    }

     void setHandler(const std::function<void(MessageCode)>& _handler)
     {
         handler = _handler;
     }

     int getPlayerNumber()
     {
         return playerNumber_;
     }

     bool getIsMyTurn()
     {
         return isMyTurn_;
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
        std::string msgCode1; std::string msgCode2;
        std::string message;
        std::istream is(&inputBuffer_);
        is >> msgCode1; is >> msgCode2;
        std::clog << "message type: " << msgCode1 << " " << msgCode2 << std::endl;

        MessageCode msgCode = messageCodesOdwrot[msgCode1 + " " + msgCode2]; std::string player_ = ""; std::string x = ""; std::string y = ""; int incominPlayerNumber_ = -1;
        switch (msgCode) {
            case MessageCode::ustawSwojNumer:
                is >> player_;
                playerNumber_ = stoi(player_);
                std::clog << "Ustawilem playerNumber_ na: " << playerNumber_ << std::endl;
                break;

            case MessageCode::zacznij:
                is >> player_;
                incominPlayerNumber_ = stoi(player_);
                if (incominPlayerNumber_ != playerNumber_) break;
                isMyTurn_ = true;
                std::clog << "Ustawilem isMyTurn_ na: " << isMyTurn_ << std::endl;
                break;

            default:
                std::clog << "jestem w default no troche mnie tu nie powinno byc niby hmmm" << std::endl;
                break;
        }

        std::getline(is, message);
        if(x != "") message = msgCode1 + " " + msgCode2 + " " + x + " " +  y + " " + player_ + message;
        else message = msgCode1 + " " + msgCode2 + " " + player_ + message;
        std::clog << "Incoming message: " << message << std::endl;

        handler(msgCode);
        read();
    }

    std::function<void(MessageCode)> handler = [](MessageCode){};
    asio::io_context io_context_;
    tcp::socket socket_;
    tcp::endpoint server_endpoint_;
    //asio::streambuf userInputBuffer;
    asio::streambuf inputBuffer_;
    int playerNumber_;
    bool isMyTurn_ = false;
};

//PS wypadaloby zmienic wystapienia int'a na inta o stalej wielkosci nie zaleznie od architektury np __int32
#endif // CLIENT_HPP_INCLUDED
