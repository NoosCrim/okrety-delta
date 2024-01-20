#include "messages.hpp"
std::map<MessageCode, std::string> messageCodes = {
    {MessageCode::strzal, "MsgCode: FIRE"},
    {MessageCode::trafiony, "MsgCode: HIT"},
    {MessageCode::niePoprawnyStrzal, "MsgCode: FFIRE"},
    {MessageCode::startTury, "MsgCode: SOT"},
    {MessageCode::przegranaGracza, "MsgCode: LOSE"},
    {MessageCode::nieTrafiony, "MsgCode: NHIT"},
    {MessageCode::ustawSwojNumer, "MsgCode: SET"},
    {MessageCode::wyszedlGracz, "MsgCode: DISC"},
    {MessageCode::ustawStatki, "MsgCode: SHIP"}
};
std::map<std::string, MessageCode> messageCodesOdwrot = {
    {"MsgCode: FIRE", MessageCode::strzal},
    {"MsgCode: HIT", MessageCode::trafiony},
    {"MsgCode: FFIRE", MessageCode::niePoprawnyStrzal},
    {"MsgCode: SOT", MessageCode::startTury},
    {"MsgCode: LOSE", MessageCode::przegranaGracza},
    {"MsgCode: NHIT", MessageCode::nieTrafiony},
    {"MsgCode: SET", MessageCode::ustawSwojNumer},
    {"MsgCode: DISC", MessageCode::wyszedlGracz},
    {"MsgCode: SHIP", MessageCode::ustawStatki}
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
    std::string startTury(int player)
    {
        return messageCodes[MessageCode::startTury] + " " + std::to_string(player) + "\n";
    }
    std::string przegranaGracza(int player)
    {
        return messageCodes[MessageCode::przegranaGracza] + " " + std::to_string(player) + "\n";
    }
    std::string ustawSwojNumer(int player)
    {
        return messageCodes[MessageCode::ustawSwojNumer] + " " + std::to_string(player) + "\n";
    }
    std::string ustawStatki(int player)
    {
        return messageCodes[MessageCode::ustawStatki] + " " + std::to_string(player) + "\n";
    }
    std::string wyszedlGracz()
    {
        return messageCodes[MessageCode::wyszedlGracz] + "\n";
    }
    std::string helloServer(const std::string& message)
    {
        return message + "\n";
    }
}
