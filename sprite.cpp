#include <iostream>
#include "sprite.hpp"
namespace AVE
{
    void Sprite::Draw(Window* window, bool flipTexV, bool flipTexH)
    {
        SDL_Rect sample = {sampleX, sampleY, sampleW, sampleH};
        SDL_FRect draw = {drawX, drawY, drawW, drawH};
        SDL_RendererFlip flip = (SDL_RendererFlip)(flipTexV?(flipTexH?(SDL_FLIP_HORIZONTAL|SDL_FLIP_VERTICAL):SDL_FLIP_VERTICAL):(flipTexH?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE));
        if(SDL_RenderCopyExF(window->GetRendererHandle(), texture->texture, &sample, &draw, angle, nullptr, flip) == -1)
            std::cerr << "Failed to draw a sprite\n\t" << SDL_GetError() << std::endl;
    }
}
