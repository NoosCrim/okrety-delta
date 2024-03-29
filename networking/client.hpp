#ifndef CLIENT_HPP_INCLUDED
#define CLIENT_HPP_INCLUDED

#include <asio.hpp>
#include <functional>
#include "messages.hpp"

using namespace asio::ip;

class AsyncClient {
public:
    AsyncClient(const std::string& server_ip, unsigned short server_port);
    ~AsyncClient();

    bool start();
    void run();
    void wylacz();

    void fire(int x, int y, int player);
    void fire(int x, int y);

    void trafil(int x, int y, int player);

    void nieTrafil(int x, int y, int player);

    void niePoprawnieStrzelil(int x, int y, int player);

    void graczPrzegral(int player);

    void ustawStatki(int player);

    void setHandler(const std::function<void(MessageCode, int, int, int)>& _handler);

    int getPlayerNumber();

    bool getIsMyTurn();

private:

    void write(const std::string&  message);

    void read();

    void handleRead();

    std::function<void(MessageCode, int x, int y, int player)> handler = [](MessageCode, int, int, int){};
    asio::io_context io_context_;
    tcp::socket socket_;
    tcp::endpoint server_endpoint_;
    //asio::streambuf userInputBuffer;
    asio::streambuf inputBuffer_;
    int playerNumber_;
    bool isMyTurn_ = false;
    bool isClosed = true;
};

//PS wypadaloby zmienic wystapienia int'a na inta o stalej wielkosci nie zaleznie od architektury np __int32
#endif // CLIENT_HPP_INCLUDED
