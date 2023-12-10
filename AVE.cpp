#include <SDL2/SDL.h>
#include <iostream>
#include "external/stb_image.h"
#include "AVE.hpp"
namespace AVE
{
    std::mutex initialized;
    bool Init()
    {
        if(!initialized.try_lock())
            return true;
        int error_code;
        if((error_code = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS))==0)
        {
            std::clog << "Initialized AVE" << std::endl;
            return true;
        }
        std::cerr << "AVE initialization failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        initialized.unlock();
        return true;

    }
    void DeInit()
    {
        SDL_Quit();
        initialized.unlock();
        std::clog << "Deinitialized AVE" << std::endl;
        return;
    }
}

