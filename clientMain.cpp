#include "gameGUI.hpp"
#include "networking/client.hpp"
int main(int argc, const char *argv[])
{
    int windowWidth = 1280, windowHeight = 960;
    const char* targetIP = "127.0.0.1";
    bool useFullscreen = false;
    unsigned int port = 2137;
    /// flag handling
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-Ws") == 0) // window size flag
        {
            windowWidth = std::stoi(argv[i+1]);
            windowHeight = std::stoi(argv[i+2]);
            i+=2;
        }
        else if(strcmp(argv[i], "-H") == 0) //host IP flag
        {
            targetIP = argv[i+1];
            i+=1;
        }
        else if(strcmp(argv[i], "-P") == 0) // host port flag
        {
            port = std::stoi(argv[i+1]);
            i+=1;
        }
        else if(strcmp(argv[i], "-Fs") == 0) // board size flag
        {
            useFullscreen = true;
        }
    }
    if(port == 0)
    {
        std::cerr << "Please, provide port to try to connect to" << std::endl;
        return 1;
    }
    if(targetIP == nullptr)
    {
        std::cerr << "Please, provide host IP" << std::endl;
        return 1;
    }
    /// end of flag handling
    AVE::Init();
    AsyncClient netClient(targetIP, port);
    OkretyGame game(netClient);
    netClient.setHandler([&game](MessageCode code, int x, int y, int playerNum){game.MessageHandler(code,x,y,playerNum);});
    if(!netClient.start())
        return 2;
    game.Open("Okrety delta", 0,0, windowWidth, windowHeight);
    if(useFullscreen)
        game.SetBorderless();
    game.StartMainLoop();
    AVE::DeInit();
    return 0;
}
