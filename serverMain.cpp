#include "gameGUI.hpp"
int main(int argc, const char *argv[])
{
    /// flag handling
    unsigned int port = 0;
    for(int i = 1; i < argc; i++) // flag handling
    {
        if(strcmp(argv[i], "-P") == 0) // host port flag
        {
            port = std::stoi(argv[i+1]);
            i+=1;
        }
    }
    if(port == 0)
    {
        std::cerr << "Please, provide port for the server" << std::endl;
        return 1;
    }
    /// end of flag handling

    AVE::Init();
    AVE::DeInit();
    return 0;
}
