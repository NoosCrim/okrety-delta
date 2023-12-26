#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <asio.hpp>

using namespace asio;

class TcpClient {
public:
    TcpClient(io_context& ioContext, const std::string& serverAddress, unsigned short serverPort)
        : ioContext_(ioContext), socket_(ioContext), serverEndpoint_(ip::make_address(serverAddress), serverPort) {
        connect();
        startReading();
    }

    void sendMessage(const std::string& message) {
        ioContext_.post([this, message]() {
            bool writeInProgress = !sendBuffer_.empty();
            sendBuffer_ += message + "\n";

            if (!writeInProgress) {
                startWriting();
            }
        });
    }

    void sendMessageInFormat(int Player, int x, int y)
    {
        const std::string msg = std::to_string(Player) + " " + std::to_string(x) + " " + std::to_string(y) + '\0';
        sendMessage(msg);
    }

private:
    void connect() {
        socket_.async_connect(serverEndpoint_, [this](const asio::error_code& error) {
            if (!error) {
                std::cout << "Connected to the server." << std::endl;
            } else {
                std::cerr << "Connection error: " << error.message() << std::endl;
            }
        });
    }

    void startReading() {
        async_read_until(socket_, receiveBuffer_, '\n', [this](const asio::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                handleRead();
                startReading(); // Continue reading
            } else {
                std::cerr << "Read error: " << error.message() << std::endl;
            }
        });
    }

    void handleRead() {
        std::unique_lock<std::mutex> lock(mutex_);
        std::istream is(&receiveBuffer_);
        std::getline(is, receivedMessage_);
        std::cout << "Received: " << receivedMessage_ << std::endl;
        //logika moment
        lock.unlock();
    }

    void startWriting() {
        async_write(socket_, buffer(sendBuffer_), [this](const asio::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                sendBuffer_.clear();
            } else {
                std::cerr << "Write error: " << error.message() << std::endl;
            }
        });
    }

private:
    io_context& ioContext_;
    ip::tcp::socket socket_;
    ip::tcp::endpoint serverEndpoint_;
    std::string sendBuffer_;
    streambuf receiveBuffer_;
    std::string receivedMessage_;
    std::mutex mutex_;
};

int main() {
    try {
        io_context ioContext;
        TcpClient client(ioContext, "127.0.0.1", 1234);

        client.sendMessage("chuj\n");


        std::string msg;
        std::cin>>msg;
        client.sendMessage(msg);

        ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
