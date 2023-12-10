#pragma once
#include <SDL2/SDL.h>
#include "window.hpp"
namespace AVE
{
    class Texture
    {
        friend class Sprite;
        SDL_Texture* texture;
    public:
        bool LoadTexture(const char* path, Window* window);
    };
}
