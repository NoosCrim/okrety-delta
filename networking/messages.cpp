#include "messages.hpp"
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
namespace Messanger
{
    std::string strzal(int x, int y, int player)
    {
        return messageCodes[MessageCode::strzal] + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(player) + "\n";
    }
    std::string trafiony(int x, int y, int player)
    {
        return messageCodes[MessageCode::trafiony] + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(player) + "\n";
    }
    std::string nieTrafiony(int x, int y, int player)
    {
        return messageCodes[MessageCode::nieTrafiony] + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(player) + "\n";
    }
    std::string niePoprawnyStrzal(int x, int y, int player)
    {
        return messageCodes[MessageCode::niePoprawnyStrzal] + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(player) + "\n";
    }
    std::string poprawnyStrzal(int x, int y, int player)
    {
        return messageCodes[MessageCode::poprawnyStrzal] + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(player) + "\n";
    }
    std::string koniecTury(int player)
    {
        return messageCodes[MessageCode::koniecTury] + " " + std::to_string(player) + "\n";
    }
    std::string przegranaGracza(int player)
    {
        return messageCodes[MessageCode::przegranaGracza] + " " + std::to_string(player) + "\n";
    }
    std::string zacznij(int player)
    {
        return messageCodes[MessageCode::zacznij] + " " + std::to_string(player) + "\n";
    }
    std::string ustawSwojNumer(int player)
    {
        return messageCodes[MessageCode::ustawSwojNumer] + " " + std::to_string(player) + "\n";
    }
    std::string helloServer(const std::string& message)
    {
        return message + "\n";
    }
}
