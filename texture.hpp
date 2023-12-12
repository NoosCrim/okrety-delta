#pragma once
#include <SDL2/SDL.h>
#include <list>
class Sprite;
namespace AVE
{
    class Window;
    class Sprite;
    class Texture
    {
        friend class Sprite;
        friend class Window;
        SDL_Texture* texture;
        Window* owner;
        std::list<Texture*>::iterator iter;
        std::list<Sprite*> mySprites;
        Texture(const char* path, Window* window);
        virtual ~Texture();
        void BindSprite(Sprite* sprite);
        void UnbindSprite(Sprite* sprite);
    public:
        Window* GetOwner() { return owner; }
    };
}
