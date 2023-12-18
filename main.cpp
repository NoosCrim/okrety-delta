#include "gameGUI.hpp"
#include <cstdio>
int main(int argc, const char *argv[])
{
    int windowWidth = 1280, windowHeight = 960;
    int boardWidth = 10, boardHeight = 10;
    const char* targetIP;
    unsigned int port;
    bool runServer;
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-Ws") == 0)
        {
            windowWidth = std::stoi(argv[i+1]);
            windowHeight = std::stoi(argv[i+2]);
            i+=2;
        }
        else if(strcmp(argv[i], "-h") == 0)
            runServer = true;
        else if(strcmp(argv[i], "-Bs") == 0)
        {
            boardWidth = std::stoi(argv[i+1]);
            boardHeight = std::stoi(argv[i+2]);
            i+=2;
        }
    }
    AVE::Init();
    mWindow window(boardWidth, boardHeight);
    AVE::Event::getGlobalMousePos = false;
    AVE::Window::SetShareEvents(true);
    window.Open("title", 0, 0, windowWidth, windowHeight);
    window.StartMainLoop();
    AVE::DeInit();
    return 0;
}
