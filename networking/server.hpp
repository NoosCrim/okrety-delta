#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <asio.hpp>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include "messages.hpp"

int turnTime = DEFAULT_TURN_TIME;

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, std::vector<std::shared_ptr<Session>>& sessions, std::mutex& sessionsMutex, int playerNumber, asio::io_context& ioContext)
        : socket_(std::move(socket)), sessions_(sessions), sessionsMutex_(sessionsMutex), myPlayerNumber(playerNumber), ioContext_(ioContext) { }

    void start() {
        {
            std::lock_guard<std::mutex> lock(sessionsMutex_);
            sessions_.push_back(shared_from_this());
            std::clog << "We have session going" << std::endl;
        }
    }

    void startReading()
    {
        doRead();
    }

    void sendMessage(std::string message) {
        //degubing line
        if(message[message.length() - 1] != '\n') message += "\n";
        asio::post(socket_.get_executor(),
            [self = shared_from_this(), message]() {
                std::clog << "Message sent to client: " << message;
                self->doWrite(message);
            });
    }

private:
    void doRead() {
        auto self(shared_from_this());
        asio::async_read_until(socket_, inputBuffer_, '\n',
            [this, self](const asio::error_code& ec, std::size_t /*length*/) {
                if (!ec) {
                    handleRead();
                } else {
                    handleDisconnect();
                }
            });
    }

    void sendToAll(const std::string& message)
    {
         std::lock_guard<std::mutex> lock(sessionsMutex_);
        // Broadcast the received message to all connected users
        for (auto& session : sessions_) {
                std::clog << "Writing to socket with endpoit: " << session->socket_.remote_endpoint() << std::endl;
                session->sendMessage(message);
        }
    }

    void handleRead() {
        //Wiadomosc jest w formie MsgCode: (Code) reszta danych
        std::string msgCode1; std::string msgCode2;
        std::string message;
        std::istream is(&inputBuffer_);
        is >> msgCode1; is >> msgCode2;

        MessageCode msgCode = messageCodesOdwrot[msgCode1 + " " + msgCode2]; std::string player_ = ""; std::string x = ""; std::string y = ""; int incominPlayerNumber_ = -1; int X; int Y; bool ignore = false;
        switch (msgCode) {
            case MessageCode::strzal:
                is >> x >> y >> player_;
                X = stoi(x);
                Y = stoi(y);
                incominPlayerNumber_ = stoi(player_);
                if(incominPlayerNumber_ == myPlayerNumber)
                {
                    std::clog << "Odpowiedni strzal dodaj jego dzialanie" << std::endl;
                }
                else
                {
                    ignore = true;
                }
                break;

            default:
                std::clog << "jestem w default no troche mnie tu nie powinno byc niby hmmm" << std::endl;
                break;
        }

        std::getline(is, message);
        if(x != "") message = msgCode1 + " " + msgCode2 + " " + x + " " +  y + " " + player_ + message;
        else message = msgCode1 + " " + msgCode2 + " " + player_ + message;
        if(!ignore) std::clog << "Incoming message: " << message << std::endl;

        // Continue reading for more messages
        doRead();
    }

    void doWrite(const std::string& message) {
        //debugging line
        if(message == "") std::clog << "Wysylam pusta wiadomosc " << std::endl;
        auto self(shared_from_this());
        // do logic write or not to write
        asio::async_write(socket_, asio::buffer(message),
            [this, self](const asio::error_code& ec, std::size_t /*length*/) {
                if (ec) {
                    handleDisconnect();
                }
            });
    }

    void handleDisconnect() {
        {
            std::lock_guard<std::mutex> lock(sessionsMutex_);
            // Remove this session from the list
            auto self(shared_from_this());
            sessions_.erase(std::remove(sessions_.begin(), sessions_.end(), shared_from_this()), sessions_.end());
            sendToAll(Messanger::wyszedlGracz());

            //wylaczanie servera (dajmy mu chwile zeby dokonczyl traffic)
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            ioContext_.stop();
            // Nie zmieniam ilosci connectow bo w trakcie gry nie mozna sie polaczyc od tego jest lobby
        }
    }

    tcp::socket socket_;
    asio::streambuf inputBuffer_;
    std::vector<std::shared_ptr<Session>>& sessions_;
    std::mutex& sessionsMutex_;
    int myPlayerNumber;
    asio::io_context& ioContext_;
};

class Server {
public:
    Server(asio::io_context& ioContext, std::size_t port, int startingPlayerIndex)
        : ioContext_(ioContext), acceptor_(ioContext, tcp::endpoint(tcp::v4(), port)), socket_(ioContext), sessionsMutex_(), timer_(ioContext) {
        startingPlayerIndex_ = startingPlayerIndex;
        doAccept();
    }

    void zmienTure()
    {
        std::lock_guard<std::mutex> lock(sessionsMutex_);
        timer_.cancel();
        startingPlayerIndex_ = startingPlayerIndex_%MAX_CONNECTIONS + 1;

        for (auto& session : sessions_) {
            session->sendMessage(Messanger::startTury(startingPlayerIndex_));
        }
        startTimer();
    }


private:
    void doAccept() {
        acceptor_.async_accept(socket_,
            [this](const asio::error_code& ec) {
                if (!ec) {
                    std::lock_guard<std::mutex> lock(conectionMutex_);
                    connections++;
                    if(connections <= MAX_CONNECTIONS)
                    {
                        std::clog << "we have established connection, it is our " << connections << " connection" << std::endl;
                        auto session_ = std::make_shared<Session>(std::move(socket_), sessions_, sessionsMutex_, connections, ioContext_);
                        session_->start(); session_->sendMessage(Messanger::ustawSwojNumer(connections));
                        if (connections == startingPlayerIndex_) session_->sendMessage(Messanger::zacznij(connections));

                        if(connections == MAX_CONNECTIONS)
                        {
                            for (auto& session : sessions_) {
                                session->startReading();
                            }
                            //zaczynam liczyc czas dla pierwszego gracza
                            startTimer();
                        }
                    }
                    else
                    {
                        std::clog << "A connection was refused, we have already established max amout of connections in number of: " << MAX_CONNECTIONS << std::endl;
                        socket_.close();
                    }
                }

                // Continue accepting new connections
                doAccept();
            });
    }

    void startTimer() {
        timer_.expires_after(std::chrono::seconds(DEFAULT_TURN_TIME)); // Ustaw czas w sekundach
        timer_.async_wait([this](const asio::error_code& ec) {
                if (!ec) {
                    std::clog << "Timer callback called!" << std::endl;
                    zmienTure();
                } else {
                    std::clog << "Timer callback error: " << ec.message() << std::endl;
                }
            });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::vector<std::shared_ptr<Session>> sessions_;
    std::mutex sessionsMutex_;
    std::mutex conectionMutex_;
    int connections = 0;
    int startingPlayerIndex_;
    asio::steady_timer timer_;
    asio::io_context& ioContext_;
};

//PS wypadaloby zmienic wystapienia int'a na inta o stalej wielkosci nie zaleznie od architektury np __int32
#endif // SERVER_HPP_INCLUDED
