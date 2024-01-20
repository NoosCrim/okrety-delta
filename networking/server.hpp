#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <iostream>
#include <asio.hpp>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include "messages.hpp"

int turnTime = DEFAULT_TURN_TIME;

using asio::ip::tcp;

struct odpowiedzNaStrzal {
    bool oczekujeOdpowiedziNaStrzal;
    int numerStrzelajacegoGracza;
};

class Server;
void zmienTure(Server& server);
void startGry(Server& server);
Server* serverPubblic; //potrzebuje uzywac zmiany tury z serwera

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, std::vector<std::shared_ptr<Session>>& sessions, std::mutex& sessionsMutex, int playerNumber, asio::io_context& ioContext, odpowiedzNaStrzal& odpowiedzNaStrzal, int& startingPlayerIndex, int& iloscStatkow, std::mutex& shipMutex, std::mutex& odpowiedzNaStrzalMutex)
        : socket_(std::move(socket)), sessions_(sessions), sessionsMutex_(sessionsMutex), myPlayerNumber(playerNumber), ioContext_(ioContext), odpowiedzNaStrzal_(odpowiedzNaStrzal),
        startingPlayerIndex_(startingPlayerIndex), iloscStatkow_(iloscStatkow),  shipMutex_(shipMutex), odpowiedzNaStrzalMutex_(odpowiedzNaStrzalMutex) { }

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

    bool getCzyUstawilStatek()
    {
        return czyUstawilStatek;
    }

    void closeServer()
    {
        //wylaczanie servera (dajmy mu chwile zeby dokonczyl traffic)
        asio::steady_timer closeTimer_(ioContext_);
        closeTimer_.expires_after(std::chrono::seconds(1)); // Ustaw czas w sekundach
        closeTimer_.async_wait([this](const asio::error_code& ec) {
            std::lock_guard<std::mutex> lock(sessionsMutex_);
            for (auto& session : sessions_) {
                session->socket_.close();
            }
            ioContext_.stop();
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

    void sendToPlayer(const std::string& message, int player)
    {
        std::clog << "Writing to socket with endpoit: " << sessions_[player - 1]->socket_.remote_endpoint() << std::endl;
        asio::async_write(sessions_[player - 1]->socket_, asio::buffer(message),
            [this](const asio::error_code& ec, std::size_t /*length*/) {
                if (ec) {
                    handleDisconnect();
                }
            });
    }

    void handleFire(int x, int y, int player)
    {
        auto self(shared_from_this());
        tcp::socket* firedSocket;
        if(player == 1) { firedSocket = &sessions_[1]->socket_; /*gracz nr 1 strzela w gracza numer 2*/}
        else { firedSocket = &sessions_[0]->socket_; /*gracz nr 2 strzela w gracza numer 1*/}
        std::string message = Messanger::strzal(x,y,player);
        asio::async_write(*firedSocket, asio::buffer(message),
            [this, firedSocket, message, player, self](const asio::error_code& ec, std::size_t /*length*/) {
            if (ec) {
                handleDisconnect();
            }
            else
            {
                std::clog << "Wyslalem wiadomosc do: " << firedSocket->remote_endpoint() << std::endl;
                std::clog << "O tresci: " << message;
                {
                    std::lock_guard<std::mutex> lock(odpowiedzNaStrzalMutex_);
                    odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal = true;
                    odpowiedzNaStrzal_.numerStrzelajacegoGracza = player;
                }
            }
        });
    }

    void handleRead() {
        try {
        //Wiadomosc jest w formie MsgCode: (Code) reszta danych
        std::string msgCode1; std::string msgCode2;
        std::string message;
        std::istream is(&inputBuffer_);
        is >> msgCode1; is >> msgCode2;

        MessageCode msgCode = messageCodesOdwrot[msgCode1 + " " + msgCode2]; std::string player_ = ""; std::string x = ""; std::string y = ""; int incominPlayerNumber_ = -1; int X; int Y; bool ignore = false;  bool czyCzekaNaStrzal = false;
        switch (msgCode) {
            case MessageCode::strzal:
                is >> x >> y >> player_;
                X = stoi(x);
                Y = stoi(y);
                incominPlayerNumber_ = stoi(player_);
                {
                    std::lock_guard<std::mutex> lock(odpowiedzNaStrzalMutex_);
                    czyCzekaNaStrzal = odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal;
                }
                if(incominPlayerNumber_ == startingPlayerIndex_ && czyCzekaNaStrzal == false)
                {
                    std::clog << "Przyszla informacja o probie strzalu w miejsce: " << X << " " << Y << " strzelal gracz o numerze: " << incominPlayerNumber_ << std::endl;
                    handleFire(X, Y, incominPlayerNumber_);
                }
                else
                {
                    std::clog << "Zignorowalem informacje o strzale, czyOczekujeOdpowiedzi: " << czyCzekaNaStrzal << std::endl;
                    std::clog << "Dane: incomingPlayerNumber: " << incominPlayerNumber_ << " a index gracza ktory aktualnie ma runde to: " << startingPlayerIndex_ << std::endl;
                    ignore = true;
                }
                break;

            case MessageCode::niePoprawnyStrzal:
                is >> x >> y >> player_;
                X = stoi(x);
                Y = stoi(y);
                incominPlayerNumber_ = stoi(player_);
                {
                    std::lock_guard<std::mutex> lock(odpowiedzNaStrzalMutex_);
                    if(odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal == false) { ignore = true; break;}
                    odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal = false; //teraz oczekuje ponownego strzalu
                }
                std::clog << "Gracz numer : " << player_ << " nie poprawnie strzelil" << std::endl;
                sendToPlayer(Messanger::niePoprawnyStrzal(X,Y,incominPlayerNumber_), incominPlayerNumber_);
                break;

            case MessageCode::trafiony:
                is >> x >> y >> player_;
                X = stoi(x);
                Y = stoi(y);
                incominPlayerNumber_ = stoi(player_);
                {
                    std::lock_guard<std::mutex> lock(odpowiedzNaStrzalMutex_);
                    if(odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal == false) { ignore = true; break;}
                    odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal = false; //teraz oczekuje ponownego strzalu
                }
                std::clog << "Gracz numer : " << player_ << " trafil" << std::endl;
                sendToPlayer(Messanger::trafiony(X,Y,incominPlayerNumber_), incominPlayerNumber_);
                zmienTure(*serverPubblic);
                break;

            case MessageCode::nieTrafiony:
                is >> x >> y >> player_;
                X = stoi(x);
                Y = stoi(y);
                incominPlayerNumber_ = stoi(player_);
                {
                    std::lock_guard<std::mutex> lock(odpowiedzNaStrzalMutex_);
                    if(odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal == false) { ignore = true; break;}
                    odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal = false; //teraz oczekuje ponownego strzalu
                }
                std::clog << "Gracz numer : " << player_ << " nie trafil" << std::endl;
                sendToPlayer(Messanger::nieTrafiony(X,Y,incominPlayerNumber_), incominPlayerNumber_);
                zmienTure(*serverPubblic);
                break;

            case MessageCode::przegranaGracza:
                is >> player_;
                incominPlayerNumber_ = stoi(player_);
                {
                    std::lock_guard<std::mutex> lock(odpowiedzNaStrzalMutex_);
                    if(odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal == false) { ignore = true; break;}
                    odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal = false; //teraz oczekuje ponownego strzalu
                }
                std::clog << "Gracz numer : " << player_ << " przegal" << std::endl;
                sendToAll(Messanger::przegranaGracza(incominPlayerNumber_));
                closeServer();
                break;

            case MessageCode::ustawStatki:
                is >> player_;
                incominPlayerNumber_ = stoi(player_);
                std::clog << "Gracz numer : " << player_ << " ustawil swoj statek" << std::endl;
                {
                    std::lock_guard<std::mutex> lock(sessionsMutex_);
                    czyUstawilStatek = true;
                    iloscStatkow_++;
                    std::clog << "Aktualna ilosc statkow: " << iloscStatkow_ << std::endl;
                    if(iloscStatkow_ == MAX_CONNECTIONS) startGry(*serverPubblic);
                }
                break;

            default:
                std::clog << "jestem w default no troche mnie tu nie powinno byc niby hmmm" << std::endl;
                break;
        }

        std::getline(is, message);
        if(x != "") message = msgCode1 + " " + msgCode2 + " " + x + " " +  y + " " + player_ + message;
        else message = msgCode1 + " " + msgCode2 + " " + player_ + message;
        if(message[message.length() - 1] != '\n') message += "\n";
        if(!ignore) std::clog << "Incoming message: " << message;
        else std::clog << "Zignorowana wiadomosc o tresci: " << message;

        // Continue reading for more messages
        doRead();
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    void doWrite(const std::string& message) {
        //debugging line
        if(message == "") std::clog << "Wysylam pusta wiadomosc " << std::endl;
        auto self(shared_from_this());
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

            closeServer();
            // Nie zmieniam ilosci connectow bo w trakcie gry nie mozna sie polaczyc od tego jest lobby
        }
    }

    tcp::socket socket_;
    asio::streambuf inputBuffer_;
    std::vector<std::shared_ptr<Session>>& sessions_;
    std::mutex& sessionsMutex_;
    std::mutex& shipMutex_;
    std::mutex& odpowiedzNaStrzalMutex_;
    int myPlayerNumber;
    asio::io_context& ioContext_;
    odpowiedzNaStrzal& odpowiedzNaStrzal_;
    int& startingPlayerIndex_;
    int& iloscStatkow_;
    bool czyUstawilStatek = false;
};

class Server {
public:
    Server(asio::io_context& ioContext, std::size_t port, int startingPlayerIndex)
        : ioContext_(ioContext), acceptor_(ioContext, tcp::endpoint(tcp::v4(), port)), socket_(ioContext), sessionsMutex_(), timer_(ioContext),  shipTimer_(ioContext) {
        startingPlayerIndex_ = startingPlayerIndex;
        odpowiedzNaStrzal_.numerStrzelajacegoGracza = -1;
        odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal = false;
        serverPubblic = this;
        doAccept();
    }

    void zmienTure()
    {
        std::lock_guard<std::mutex> lock(timerMutex_);
        odpowiedzNaStrzal_.oczekujeOdpowiedziNaStrzal = false;
        timer_.cancel();
        startingPlayerIndex_ = startingPlayerIndex_%MAX_CONNECTIONS + 1;

        zaznijTure();
        startTimer();
    }

    void graczResetujeTimer()
    {
        {
            std::lock_guard<std::mutex> lock(timerMutex_);
            timerCallbackIndex++;
            std::clog << "aktualny index przy zmianie tury: " << timerCallbackIndex << std::endl;
        }
        zmienTure();
    }

    void zaznijTure()
    {
        std::lock_guard<std::mutex> lock(sessionsMutex_);
        for (auto& session : sessions_) {
            std::clog << "wysylana wiadomosc: " << Messanger::startTury(startingPlayerIndex_);
            session->sendMessage(Messanger::startTury(startingPlayerIndex_));}
    }

    void startGry()
    {
        std::clog << "Zaczynam rozgrywke prosze zapiac pasy." << std::endl;
        //zaczynam ture
        for (auto& session : sessions_) {
            std::clog << "wysylana wiadomosc: " << Messanger::startTury(startingPlayerIndex_);
            session->sendMessage(Messanger::startTury(startingPlayerIndex_));
        }
        //zaczynam liczyc czas dla pierwszego gracza
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
                        auto session_ = std::make_shared<Session>(std::move(socket_), sessions_, sessionsMutex_, connections, ioContext_, odpowiedzNaStrzal_, startingPlayerIndex_, iloscStatkow, shipMutex_, odpowiedzNaStrzalMutex_);
                        session_->start(); session_->sendMessage(Messanger::ustawSwojNumer(connections));

                        if(connections == MAX_CONNECTIONS)
                        {
                            for (auto& session : sessions_) {
                                session->startReading();
                            }

                            shipTimer_.expires_after(std::chrono::seconds(DEFAULT_SHIP_PLACEMENT_TIME));
                            shipTimer_.async_wait([this](const asio::error_code& ec) {
                                if (!ec) {
                                    if(iloscStatkow != MAX_CONNECTIONS)
                                    {
                                        for(unsigned int i = 0; i < sessions_.size(); i++)
                                        {
                                            if(sessions_[i]->getCzyUstawilStatek() == true)
                                            {
                                                //bo gracz i+1 != i+2 więc dostanie on wygrana gdyz przegral inny gracz
                                                 sessions_[i]->sendMessage(Messanger::przegranaGracza(i+2));
                                            }
                                            else
                                            {
                                                sessions_[i]->sendMessage(Messanger::przegranaGracza(i+1));
                                            }
                                        }
                                        sessions_[0]->closeServer();
                                    }
                                    else
                                    {
                                        std::clog << "Ship Timer Callback: Statki zostaly ustawione pomyslnie (juz po, mozna zignorowac)." << std::endl;
                                    }
                                }
                                else {
                                     std::clog << "Ship Timer callback error: " << ec.message() << std::endl;
                                }
                            });
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
                    bool doICallback = false;
                    {
                        std::lock_guard<std::mutex> lock(timerMutex_);
                        if(timerCallbackIndex == 0) doICallback = true;
                    }

                    if(doICallback) {
                        std::clog << "Timer callback called!" << std::endl;
                        zmienTure();
                    }
                    else
                    {
                        std::clog << "Timer callback: element usuniety z kolejki" << std::endl;
                        {
                            std::lock_guard<std::mutex> lock(timerMutex_);
                            timerCallbackIndex--;
                            std::clog << "aktualny index: " << timerCallbackIndex << std::endl;
                            startTimer();
                        }
                    }
                } else {
                    if(ec.message() == "Operation aborted.")
                    {
                        std::clog << "Timer callback: element usuniety z kolejki (abort)" << std::endl;
                        {
                            std::lock_guard<std::mutex> lock(timerMutex_);
                            timerCallbackIndex--;
                            std::clog << "aktualny index: " << timerCallbackIndex << std::endl;
                        }
                    }
                    else
                    std::clog << "Timer callback error: " << ec.message() << std::endl;
                }
            });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::vector<std::shared_ptr<Session>> sessions_;
    std::mutex sessionsMutex_;
    std::mutex conectionMutex_;
    std::mutex timerMutex_;
    std::mutex shipMutex_;
    std::mutex odpowiedzNaStrzalMutex_;
    int connections = 0;
    int startingPlayerIndex_;
    asio::steady_timer timer_;
    asio::steady_timer shipTimer_;
    asio::io_context& ioContext_;
    odpowiedzNaStrzal odpowiedzNaStrzal_;
    int timerCallbackIndex = 0;
    int iloscStatkow = 0;
};

void zmienTure(Server& server)
{
    server.graczResetujeTimer();
}

void startGry(Server& server)
{
    server.startGry();
}

//PS wypadaloby zmienic wystapienia int'a na inta o stalej wielkosci nie zaleznie od architektury np __int32
#endif // SERVER_HPP_INCLUDED
