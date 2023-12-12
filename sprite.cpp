#include <iostream>
#include "sprite.hpp"
#include "window.hpp"
#include "texture.hpp"
namespace AVE
{
    Sprite::Sprite(Texture* tex, int _sX, int _sY, int _sW, int _sH, float _x, float _y, float _w, float _h, float _angle, bool _flipH, bool _flipV):
        Active(tex->owner),
        texture(tex), sX(_sX), sY(_sY), sW(_sW), sH(_sH), x(_x), y(_y), w(_w), h(_h), angle(_angle), flipH(_flipH), flipV(_flipV)
    {
        tex->BindSprite(this);
    }
    Sprite* Sprite::CreateSprite(Texture* tex, int sX, int sY, int sW, int sH, float x, float y, float w, float h, float angle, bool flipH, bool flipV)
    {
        return new Sprite(tex, sX,sY,sW,sH,x,y,w,h,angle,flipH, flipV);
    }
    void Sprite::Draw()
    {
        SDL_Rect sample = {sX, sY, sW, sH};
        SDL_FRect draw = {x, y, w, h};
        SDL_RendererFlip flip = (SDL_RendererFlip)(flipV?(flipH?(SDL_FLIP_HORIZONTAL|SDL_FLIP_VERTICAL):SDL_FLIP_VERTICAL):(flipH?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE));
        if(SDL_RenderCopyExF(texture->owner->GetRendererHandle(), texture->texture, &sample, &draw, angle, nullptr, flip) == -1)
            std::cerr << "Failed to draw a sprite\n\t" << SDL_GetError() << std::endl;
    }
    Sprite::~Sprite()
    {
        texture->UnbindSprite(this);
    }
}
