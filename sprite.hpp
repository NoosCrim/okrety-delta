#pragma once
#include "texture.hpp"
namespace AVE
{
    class Sprite
    {
    public:
        float angle;
        float drawX = 0.0f, drawY = 0.0f, drawW = 0.0f, drawH = 0.0f;
        int sampleX = 0, sampleY = 0, sampleW = 0, sampleH = 0;
        Texture* texture;
        void Draw(Window* window, bool flipTexV, bool flipTexH);

    };
}
