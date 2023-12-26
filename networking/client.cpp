#include <iostream>
#include <asio.hpp>

using namespace asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

int main() {
    asio::io_service io_service;
//socket creation
     tcp::socket socket(io_service);
//connection
     socket.connect( tcp::endpoint( asio::ip::address::from_string("127.0.0.1"), 1234 ));
// request/message from client
     string msg = "Hello from Client!\n";
     std::cin>>msg; msg += "\n";

     msg = "abcde\n" + '\0';
     asio::error_code error;
     asio::write( socket, asio::buffer(msg), error );
     if( !error ) {
        cout << "Client sent hello message!" << endl;
     }
     else {
        cout << "send failed: " << error.message() << endl;
     }
 // getting response from server
    asio::streambuf receive_buffer;
    asio::read(socket, receive_buffer, asio::transfer_all(), error);
    if( error && error != asio::error::eof ) {
        cout << "receive failed: " << error.message() << endl;
    }
    else {
        const char* data = asio::buffer_cast<const char*>(receive_buffer.data());
        cout << data << endl;
    }
    return 0;
}
