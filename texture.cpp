#include <iostream>
#include <SDL2/SDL_image.h>
#include "texture.hpp"
namespace AVE
{
    bool Texture::LoadTexture(const char* path, Window* window)
    {
        if((texture = IMG_LoadTexture(window->GetRendererHandle(), path)))
        {
            std::clog << "Loaded texture " << path << std::endl;
            return true;
        }
        std::cerr << "Failed to load texture " << path << std::endl;
        std::cerr << '\t' << SDL_GetError() << std::endl;
        return false;
    }
}
