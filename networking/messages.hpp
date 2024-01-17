#ifndef MESSAGES_HPP_INCLUDED
#define MESSAGES_HPP_INCLUDED

#include "../gameCONSTS.hpp"
#include <map>
#include <string>

enum class MessageCode {
    strzal = 0,
    trafiony = 1,
    niePoprawnyStrzal = 2,
    poprawnyStrzal = 3,
    startTury = 4,
    przegranaGracza = 5,
    zacznij = 6,
    nieTrafiony = 7,
    ustawSwojNumer = 8,
    wyszedlGracz = 9
};

extern std::map<MessageCode, std::string> messageCodes;

extern std::map<std::string, MessageCode> messageCodesOdwrot;

namespace Messanger {
    //tworzy wiadomosc o tresci: numer pola na mapie, informacja o numerze gracza który strzela
    std::string strzal(int x, int y, int player);

    //tworzy wiadomosc o tresci: numer pola na mapie, informacja o numerze gracza który zostal trafiony
    std::string trafiony(int x, int y, int player);

    //tworzy wiadomosc o tresci: numer pola na mapie, informacja o numerze gracza który zostal nie trafiony
    std::string nieTrafiony(int x, int y, int player);

    //tworzy wiadomosc o tresci:numer pola na mapie, informacja o numerze gracza który strzelił w miejsce, w którym juz zostal strzał oddany
    std::string niePoprawnyStrzal(int x, int y, int player);

    //tworzy wiadomosc o tresci:numer pola na mapie, informacja o numerze gracza który strzelił w prawidlowe miejsce
    std::string poprawnyStrzal(int x, int y, int player);

    //tworzy wiadomosc o tresci: mowi o numerze gracza ktoremu skonczyla sie tura
    std::string startTury(int player);

    //tworzy wiadomosc o tresci: mowi o numerze gracza ktory przegral
    std::string przegranaGracza(int player);

    //tworzy wiadomosc o tresci: mowi o graczu ktory zaczyna rozgrywke, a wlasciwie mowi mu ze to on zaczyna
    std::string zacznij(int player);

    //tworzy wiadomosc o tresci: mowi graczu o tym jaki numer przypisal mu server
    std::string ustawSwojNumer(int player);

    //tworzy wiadomosc o trescji: mowi o tym ze wyszedl gracz i jego numer
    std::string wyszedlGracz();


    //do debugowania
    //tworzy wiadomosc o tresci: wyslanego stringa dodaje \n na koncu
    std::string helloServer(const std::string& message);
};

#endif // MESSAGES_HPP_INCLUDED
