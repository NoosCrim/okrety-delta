#include <iostream>
#include <asio.hpp>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include "thsqueue.hpp"
#include "logika.hpp"

typedef std::uint32_t mint;

using namespace asio;
using ip::tcp;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

class Serwer
{
    private:
        asio::io_service* Server_io_service;
        tcp::acceptor acceptor_;
        tcp::socket socket_; //Server socket
        mint connections;
        mint presentNumberOfConnections = 0;
        enum { max_length = 1024 };
        vector<std::shared_ptr<tcp::socket>> clientSockets;
        ThreadSafeQueue<string> Q;
        std::mutex mutex_;
        std::mutex Continue_Mutex;
        bool flag2;
        std::condition_variable cv;

        void handleConnection(std::shared_ptr<tcp::socket> socket)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            presentNumberOfConnections += 1;
            clientSockets.push_back(socket);
            lock.unlock();
        }

        void startAccept()
        {
            // Create a shared_ptr to manage the lifetime of the socket
            auto socketPtr = std::make_shared<tcp::socket>(*Server_io_service);

            acceptor_.async_accept(*socketPtr, [this, socketPtr](const asio::error_code& ec) {
            if (!ec) {
                cout << "New connection established." << endl;
                handleConnection(socketPtr);
            } else {
                cerr << "Error accepting connection: " << ec.message() << endl;
            }

            // Continue to accept new connections
            if(presentNumberOfConnections < connections) startAccept();
            if(presentNumberOfConnections == connections) {
                startListening();
                write();}
            });
        }

        void read(std::shared_ptr<tcp::socket> socket, int id) {
            streambuf receiveBuffer_;
            cout << "Czekam na " << id << "\n";
            async_read_until(socket_, receiveBuffer_, '\0',
                   [this, &receiveBuffer_, &socket, &id](const asio::error_code& error, std::size_t bytes_transferred) {
                             std::string data = asio::buffer_cast<const char*>(receiveBuffer_.data());
                       if (!error) {
                           std::cout << "Received: " << data << std::endl;
                           Q.push(data);
                            {
                                std::lock_guard<std::mutex> lock(Continue_Mutex);
                                flag2 = true;
                            }
                            cv.notify_one();
                            read(socket, id);
                       } else {
                           std::cerr << "Error reading data: " << error.message() << " Number of client socket: " << id << std::endl;
                       }
                   });
        }

        void startListening()
        {
            for(int i = 0; i < clientSockets.size(); i++)
            {
                read(clientSockets[i], i);
                cout << "started listening to client " << i << std::endl;
            }
        }

        void write(){
            cout << "Wypisujemy tutaj kurde\n";
            while(!Q.isEmpty())
            {
                string elem = Q.pop();
                for(int i = 0; i < clientSockets.size(); i++)
                {
                    //logika
                    async_write(*clientSockets[i], asio::buffer(elem),
                        [this](const asio::error_code& error, std::size_t bytes_transferred) {
                                if(error)
                                {
                                    std::cerr << "Error writting data: " << error.message() << std::endl;
                                    //handle that
                                }
                                else
                                {
                                    //dziala
                                }
                    });
                }
            }

            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv.wait(lock, [this] { return flag2; });
                write();
            }
        }

    public:
        Serwer(asio::io_service& io_service, int port, short conn)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
            socket_(io_service)
            {
                Server_io_service = &io_service;
                connections = conn;
                startAccept();
            }
};


int main() {
    asio::io_service io_service;
    Serwer server(io_service, 1234, 2);

    io_service.run();

    return 0;
}
