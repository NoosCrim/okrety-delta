#pragma once
#include <list>
#include "common.hpp"
namespace AVE
{
    class Texture;
    class Sprite : public Active
    {
        friend class Texture;
        std::list<Sprite*>::iterator texIter, winIter;
        Texture *texture;
    protected:
        //Sprite should always be created using new, and then bound to Texture, so it can later be destroyed
        Sprite(Texture* tex, int sX, int sY, int sW, int sH, float x, float y, float w, float h, float angle, bool flipH, bool flipV);
        virtual void OnStart(){};
        virtual void OnUpdate() override{};
    public:
        virtual ~Sprite();
        static Sprite* CreateSprite(Texture* tex, int sX = 0, int sY = 0, int sW = 0, int sH = 0, float x = 0, float y = 0, float w = 0, float h = 0, float angle = 0, bool flipH = 0, bool flipV = 0);
        static Sprite* CreateSprite(const Sprite* other);
        int sX = 0, sY = 0, sW = 0, sH = 0; // texture sample coordinates and size
        float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f; // window space display sprite coordinates and size
        float angle; // display angle
        bool flipH, flipV; // whether sprite should be drawn flipped
        bool visible = true; // whether sprite should be drawn on owner draw call
        void Draw();
        Texture* GetTexture() { return texture; };
        Window* GetWindow();
    };
}
