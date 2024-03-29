#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
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
            if(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_WEBP)==(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_WEBP))
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

