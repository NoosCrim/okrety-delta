#include "gameGUI.hpp"
int main(int argc, const char *argv[])
{
    int windowWidth = 1280, windowHeight = 960;
    int boardWidth = 10, boardHeight = 10;
    const char* targetIP;
    unsigned int port;
    for(int i = 1; i < argc; i++) // flag handling
    {
        if(strcmp(argv[i], "-Ws") == 0) // window size flag
        {
            windowWidth = std::stoi(argv[i+1]);
            windowHeight = std::stoi(argv[i+2]);
            i+=2;
        }
        else if(strcmp(argv[i], "-Bs") == 0) // board size flag
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
