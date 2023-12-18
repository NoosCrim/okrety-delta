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
    Sprite* Sprite::CreateSprite(Texture* tex, int _sX, int _sY, int _sW, int _sH, float _x, float _y, float _w, float _h, float _angle, bool _flipH, bool _flipV)
    {
        return new Sprite(tex, _sX,_sY,_sW,_sH,_x,_y,_w,_h,_angle,_flipH,_flipV);
    }
    Sprite* Sprite::CreateSprite(const Sprite* other)
    {
        return CreateSprite(other->texture,other->sX,other->sY,other->sW,other->sH,other->x,other->y,other->w,other->h,other->angle,other->flipH,other->flipV);
    }
    void Sprite::Draw()
    {
        if(!visible)
            return;
        SDL_Rect sample = {sX, sY, sW, sH};
        SDL_FRect draw = {x, y, w, h};
        //std::cout << sX << ' ' << sY << ' ' << sW << ' ' << sH << std::endl;
        //std::cout << x << ' ' << y << ' ' << w << ' ' << h << std::endl;
        SDL_RendererFlip flip = (SDL_RendererFlip)(flipV?(flipH?(SDL_FLIP_HORIZONTAL|SDL_FLIP_VERTICAL):SDL_FLIP_VERTICAL):(flipH?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE));
        if(SDL_RenderCopyExF(texture->owner->GetRendererHandle(), texture->texture, &sample, &draw, angle, nullptr, flip) == -1)
            std::cerr << "Failed to draw a sprite\n\t" << SDL_GetError() << std::endl;
    }
    Sprite::~Sprite()
    {
        texture->UnbindSprite(this);
    }
    Window* Sprite::GetWindow() { return texture->owner; };
}
