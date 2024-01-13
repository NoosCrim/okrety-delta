#ifndef MESSAGES_HPP_INCLUDED
#define MESSAGES_HPP_INCLUDED
#include <map>
#include <string>

enum class MessageCode {
    strzal = 0,
    trafiony = 1,
    niePoprawnyStrzal = 2,
    poprawnyStrzal = 3,
    koniecTury = 4,
    przegranaGracza = 5,
    zdrowiaGraczy = 6
};

std::map<MessageCode, std::string> messageCodes = {
    {MessageCode::strzal, "MsgCode: FIRE"},
    {MessageCode::trafiony, "MsgCode: HIT"},
    {MessageCode::niePoprawnyStrzal, "MsgCode: FFIRE"},
    {MessageCode::poprawnyStrzal, "MsgCode: TFIRE"},
    {MessageCode::koniecTury, "MsgCode: EOT"},
    {MessageCode::przegranaGracza, "MsgCode: LOSE"},
    {MessageCode::zdrowiaGraczy, "MsgCode: HP"}
};

class Messanger {
public:
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

    //tworzy wiadomosc o tresci: mowi o zdrowiu graczy na poczatku rozgrywki
    std::string zdrowiaGraczy(int hp)
    {
        return messageCodes[MessageCode::zdrowiaGraczy] + " " + std::to_string(hp) + "\n";
    }

    //do debugowania
    //tworzy wiadomosc o tresci: wyslanego stringa dodaje \n na koncu
    std::string helloServer(const std::string& message)
    {
        return message + "\n";
    }
};

#endif // MESSAGES_HPP_INCLUDED
