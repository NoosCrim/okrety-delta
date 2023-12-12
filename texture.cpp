#include <iostream>
#include <SDL2/SDL_image.h>
#include "texture.hpp"
#include "window.hpp"
#include "sprite.hpp"
namespace AVE
{
    Texture::Texture(const char* path, Window* window) : owner(window)
    {
        iter = owner->myTextures.insert(owner->myTextures.begin(), this);
        if((texture = IMG_LoadTexture(owner->GetRendererHandle(), path)))
        {
            std::clog << "Loaded texture " << path << " to " << this <<std::endl;
        }
        else
        {
            std::cerr << "Failed to load texture " << path << std::endl;
            std::cerr << '\t' << SDL_GetError() << std::endl;
        }

    }
    Texture::~Texture()
    {
        std::clog << "Unloaded texture " << this << std::endl;
        for(std::list<Sprite*>::iterator iter = mySprites.begin(); iter != mySprites.end(); )
        {
            std::list<Sprite*>::iterator curr = iter++;
            delete (*curr);
        }

        if(texture)
            SDL_DestroyTexture(texture);
        owner->myTextures.erase(iter);
    }
    void Texture::BindSprite(Sprite* sprite)
    {
        sprite->iter = mySprites.insert(mySprites.begin(), sprite);
        sprite->texture = this;
    }
    void Texture::UnbindSprite(Sprite* sprite)
    {
        mySprites.erase(sprite->iter);
    }
}
