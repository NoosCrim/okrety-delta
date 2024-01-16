#ifndef MESSAGES_HPP_INCLUDED
#define MESSAGES_HPP_INCLUDED

#define serverPORT 2137
#include <map>
#include <string>

enum class MessageCode {
    strzal = 0,
    trafiony = 1,
    niePoprawnyStrzal = 2,
    poprawnyStrzal = 3,
    koniecTury = 4,
    przegranaGracza = 5,
    zacznij = 6,
    nieTrafiony = 7,
    ustawSwojNumer = 8
};

std::map<MessageCode, std::string> messageCodes = {
    {MessageCode::strzal, "MsgCode: FIRE"},
    {MessageCode::trafiony, "MsgCode: HIT"},
    {MessageCode::niePoprawnyStrzal, "MsgCode: FFIRE"},
    {MessageCode::poprawnyStrzal, "MsgCode: TFIRE"},
    {MessageCode::koniecTury, "MsgCode: EOT"},
    {MessageCode::przegranaGracza, "MsgCode: LOSE"},
    {MessageCode::zacznij, "MsgCode: START"},
    {MessageCode::nieTrafiony, "MsgCode: NHIT"},
    {MessageCode::ustawSwojNumer, "MsgCode: SET"}
};

std::map<std::string, MessageCode> messageCodesOdwrot = {
    {"MsgCode: FIRE", MessageCode::strzal},
    {"MsgCode: HIT", MessageCode::trafiony},
    {"MsgCode: FFIRE", MessageCode::niePoprawnyStrzal},
    {"MsgCode: TFIRE", MessageCode::poprawnyStrzal},
    {"MsgCode: EOT", MessageCode::koniecTury},
    {"MsgCode: LOSE", MessageCode::przegranaGracza},
    {"MsgCode: START", MessageCode::zacznij},
    {"MsgCode: NHIT", MessageCode::nieTrafiony},
    {"MsgCode: SET", MessageCode::ustawSwojNumer}
};

namespace Messanger {
    //tworzy wiadomosc o tresci: numer pola na mapie, informacja o numerze gracza który strzela
    std::string strzal(int x, int y, int player)
    {
        return messageCodes[MessageCode::strzal] + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(player) + "\n";
    }

    //tworzy wiadomosc o tresci: numer pola na mapie, informacja o numerze gracza który zostal trafiony
    std::string trafiony(int x, int y, int player)
    {
        return messageCodes[MessageCode::trafiony] + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(player) + "\n";
    }

    //tworzy wiadomosc o tresci: numer pola na mapie, informacja o numerze gracza który zostal nie trafiony
    std::string nieTrafiony(int x, int y, int player)
    {
        return messageCodes[MessageCode::nieTrafiony] + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(player) + "\n";
    }

    //tworzy wiadomosc o tresci:numer pola na mapie, informacja o numerze gracza który strzelił w miejsce, w którym juz zostal strzał oddany
    std::string niePoprawnyStrzal(int x, int y, int player)
    {
        return messageCodes[MessageCode::niePoprawnyStrzal] + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(player) + "\n";
    }

    //tworzy wiadomosc o tresci:numer pola na mapie, informacja o numerze gracza który strzelił w prawidlowe miejsce
    std::string poprawnyStrzal(int x, int y, int player)
    {
        return messageCodes[MessageCode::poprawnyStrzal] + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(player) + "\n";
    }

    //tworzy wiadomosc o tresci: mowi o numerze gracza ktoremu skonczyla sie tura
    std::string koniecTury(int player)
    {
        return messageCodes[MessageCode::koniecTury] + " " + std::to_string(player) + "\n";
    }

    //tworzy wiadomosc o tresci: mowi o numerze gracza ktory przegral
    std::string przegranaGracza(int player)
    {
        return messageCodes[MessageCode::przegranaGracza] + " " + std::to_string(player) + "\n";
    }

    //tworzy wiadomosc o tresci: mowi o graczu ktory zaczyna rozgrywke, a wlasciwie mowi mu ze to on zaczyna
    std::string zacznij(int player)
    {
        return messageCodes[MessageCode::zacznij] + " " + std::to_string(player) + "\n";
    }

    //tworzy wiadomosc o tresci: mowi graczu o tym jaki numer przypisal mu server
    std::string ustawSwojNumer(int player)
    {
        return messageCodes[MessageCode::ustawSwojNumer] + " " + std::to_string(player) + "\n";
    }


    //do debugowania
    //tworzy wiadomosc o tresci: wyslanego stringa dodaje \n na koncu
    std::string helloServer(const std::string& message)
    {
        return message + "\n";
    }
};

#endif // MESSAGES_HPP_INCLUDED
