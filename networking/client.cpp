#include "client.hpp"
#include <iostream>
AsyncClient::AsyncClient(const std::string& server_ip, unsigned short server_port):
    socket_(io_context_),
    server_endpoint_(asio::ip::make_address(server_ip), server_port)
    {

    }
bool AsyncClient::start()
{
    try {
        // Start the asynchronous connection
        socket_.connect(server_endpoint_);
        std::clog << "Connected to the server was succesfull" << std::endl;

        // Start asynchronous read
        read();

        // Run the io_context to keep the program alive
        return true;
   } catch (std::exception& e) {
        std::clog << "Connection refused: " << e.what() << std::endl;
        return false;
    }
}
void AsyncClient::run()
{
    io_context_.run();
}
void AsyncClient::fire(int x, int y, int player)
{
    write(Messanger::strzal(x,y,player));
}
void AsyncClient::fire(int x, int y)
{
    write(Messanger::strzal(x,y,playerNumber_));
}
void AsyncClient::trafil(int x, int y, int player)
{
    write(Messanger::trafiony(x,y,player));
}
void AsyncClient::nieTrafil(int x, int y, int player)
{
    write(Messanger::nieTrafiony(x,y,player));
}
void AsyncClient::niePoprawnieStrzelil(int x, int y, int player)
{
    write(Messanger::niePoprawnyStrzal(x,y,player));
}
void AsyncClient::graczPrzegral(int player)
{
    write(Messanger::przegranaGracza(player));
}
void AsyncClient::setHandler(const std::function<void(MessageCode, int, int, int)>& _handler)
{
    handler = _handler;
}
int AsyncClient::getPlayerNumber()
{
    return playerNumber_;
}
bool AsyncClient::getIsMyTurn()
{
    return isMyTurn_;
}
void AsyncClient::write(const std::string&  message)
{
    // Data to be sent
    std::clog << "Writing: " << message << std::endl;

    // Start asynchronous write operation
    asio::async_write(socket_, asio::buffer(message),
                      [this, message](std::error_code ec, std::size_t /*length*/) {
        if (!ec) {
            std::clog << "Sent: " << message;

        } else {
            std::clog << "Write error: " << ec.message() << std::endl;
        }
    });
}
void AsyncClient::read()
{
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
void AsyncClient::handleRead()
{
    std::string msgCode1; std::string msgCode2;
    std::string message;
    std::istream is(&inputBuffer_);
    is >> msgCode1; is >> msgCode2;
    std::clog << "message type: " << msgCode1 << " " << msgCode2 << std::endl;

    MessageCode msgCode = messageCodesOdwrot[msgCode1 + " " + msgCode2]; std::string player_ = ""; std::string x = ""; std::string y = ""; int incominPlayerNumber_ = -1; int X = -1; int Y = -1;
    switch (msgCode) {
        case MessageCode::ustawSwojNumer:
            is >> player_;
            playerNumber_ = stoi(player_);
            std::clog << "Ustawilem playerNumber_ na: " << playerNumber_ << std::endl;
            break;

        case MessageCode::wyszedlGracz:
            std::clog << "Wyszedl przeciwnik wiec koncze dzialanie" << std::endl;
            socket_.close();
            io_context_.stop();
            break;

        case MessageCode::startTury:
            is >> player_;
            incominPlayerNumber_ = stoi(player_);
            if (incominPlayerNumber_ == playerNumber_) isMyTurn_ = true;
            else isMyTurn_ = false;
            std::clog << "Ustawilem isMyTurn_ na: " << isMyTurn_ << " bo ture zaczyna teraz gracz nr: " << incominPlayerNumber_ << std::endl;
            break;

        case MessageCode::strzal:
            is >> x >> y >> player_;
            X = stoi(x);
            Y = stoi(y);
            incominPlayerNumber_ = stoi(player_);
            std::clog << "Odebralem wiadomosc o strzale o w miejscu: " << X << " " << Y << " od gracza numer: " << incominPlayerNumber_ << std::endl;
            break;

        case MessageCode::niePoprawnyStrzal:
            is >> x >> y >> player_;
            X = stoi(x);
            Y = stoi(y);
            incominPlayerNumber_ = stoi(player_);
            std::clog << "Odebralem wiadomosc o nie poprawnym strzale o w miejscu: " << X << " " << Y << " od gracza numer: " << incominPlayerNumber_ << std::endl;
            break;

        default:
            std::clog << "jestem w default no troche mnie tu nie powinno byc niby hmmm" << std::endl;
            break;
    }

    std::getline(is, message);
    if(x != "") message = msgCode1 + " " + msgCode2 + " " + x + " " +  y + " " + player_ + message;
    else message = msgCode1 + " " + msgCode2 + " " + player_ + message;
    std::clog << "Incoming message: " << message;

    handler(msgCode, X, Y, incominPlayerNumber_);
    read();
}

