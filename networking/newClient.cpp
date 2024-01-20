#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include "client.hpp"

using std::cin;
AsyncClient*  client_;
void clientThreadFunction() {
    // Replace "127.0.0.1" and 12345 with your server's IP address and port
    AsyncClient client("127.0.0.1", SERVER_PORT);
    if (client.start() == false) {
        std::clog << "Unsuccessful connection" << std::endl;
    } else {
        std::clog << "Successful connection" << std::endl;
        client_ = &client;

        client.run();
    }
}

int main() {
    try {
         // Start the client in a separate thread
        std::thread clientThread(clientThreadFunction);

        asio::io_context ioContext;

        // Tworzenie timera
        asio::steady_timer timer(ioContext, asio::chrono::seconds(5));

        // Funkcja obsługująca zdarzenie timera
        std::function<void(const asio::error_code&)> timerHandler = [&](const asio::error_code& ec) {
        if (!ec) {
            std::cout << "podaj rodzaj interakcji" << std::endl; int x,y,player;
            std::cout << "1 - strzal,   2 - niePoprawnieStrzelil,   3-Niedostal,   4 - dostal,  5 - Przegral  6 - Ustaw statek" << std::endl;
            int z; cin>>z;
            switch(z)
            {
            case 1:
                cin>>x>>y>>player;
                client_->fire(x,y,player);
                break;
            case 2:
                cin>>x>>y>>player;
                client_->niePoprawnieStrzelil(x,y,player);
                break;
            case 3:
                cin>>x>>y>>player;
                client_->nieTrafil(x,y,player);
                break;
            case 4:
                cin>>x>>y>>player;
                client_->trafil(x,y,player);
                break;
            case 5:
                cin>>player;
                client_->graczPrzegral(player);
                break;
            case 6:
                cin>>player;
                client_->ustawStatki(player);
                break;
            }
            // Dodaj kod do wysyłania wiadomości na strumień wejścia tutaj

            // Ponowne ustawienie timera
            timer.expires_after(asio::chrono::seconds(5));
            timer.async_wait(timerHandler);
        }
        };

        // Ustawienie pierwszego wywołania timera
        timer.async_wait(timerHandler);

        // Uruchomienie serwera w pętli obsługi zdarzeń
        ioContext.run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
