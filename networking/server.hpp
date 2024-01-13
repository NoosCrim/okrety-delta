#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <asio.hpp>
#include <deque>
#include <memory>
#include <mutex>
#include "messages.hpp"

#define MAX_CONNECTIONS 5
#define DEFAULT_TURN_TIME 30
int turnTime = DEFAULT_TURN_TIME;

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, std::deque<std::shared_ptr<Session>>& sessions, std::mutex& sessionsMutex)
        : socket_(std::move(socket)), sessions_(sessions), sessionsMutex_(sessionsMutex) { }

    void start() {
        {
            std::lock_guard<std::mutex> lock(sessionsMutex_);
            sessions_.push_back(shared_from_this());
            std::clog << "We have session going" << std::endl;
        }
        doRead();
    }

    void sendMessage(const std::string& message) {
        asio::post(socket_.get_executor(),
            [self = shared_from_this(), message]() {
                std::clog << "Message sent to client: " << message << std::endl;
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

    void handleRead() {
        std::string message;
        std::istream is(&inputBuffer_);
        std::getline(is, message);
        std::clog << "Incoming message: " << message << std::endl;

        {
            std::lock_guard<std::mutex> lock(sessionsMutex_);
            // Broadcast the received message to all connected users
            for (auto& session : sessions_) {
                    std::clog << "Writing to socket with endpoit: " << session->socket_.remote_endpoint() << std::endl;
                    session->sendMessage(message);
            }
        }

        // Continue reading for more messages
        doRead();
    }

    void doWrite(const std::string& message) {
        auto self(shared_from_this());
        // do logic write or not to write
        asio::async_write(socket_, asio::buffer(message + "\n"),
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
            std::clog << "Disconected session with id: " << self->socket_.remote_endpoint() << std::endl;
            sessions_.erase(std::remove(sessions_.begin(), sessions_.end(), shared_from_this()), sessions_.end());
            // Nie zmieniam ilosci connectow bo w trakcie gry nie mozna sie polaczyc od tego jest lobby
        }
    }

    tcp::socket socket_;
    asio::streambuf inputBuffer_;
    std::deque<std::shared_ptr<Session>>& sessions_;
    std::mutex& sessionsMutex_;
};

class Server {
public:
    Server(asio::io_context& ioContext, std::size_t port)
        : acceptor_(ioContext, tcp::endpoint(tcp::v4(), port)),
          socket_(ioContext),
          sessionsMutex_() {
        doAccept();
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
                        std::make_shared<Session>(std::move(socket_), sessions_, sessionsMutex_)->start();
                    }
                    else
                    {
                        std::clog << "A connection was refused, we have already established max amout of connections in number of: " << MAX_CONNECTIONS << std::endl;
                    }
                }

                // Continue accepting new connections
                doAccept();
            });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::deque<std::shared_ptr<Session>> sessions_;
    std::mutex sessionsMutex_;
    std::mutex conectionMutex_;
    int connections = 0;
    Messanger msg;
};

#endif // SERVER_HPP_INCLUDED
