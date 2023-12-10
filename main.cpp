#include <iostream>
#include "AVE.hpp"
#include <SDL2/SDL.h>
using namespace std;
class mWindow : public AVE::Window
{
public:
    AVE::Texture texture;
    AVE::Sprite sprite;
    float xV = 2.5, yV = 2.5;
    float xP = 0.0, yP = 0.0;
protected:
    virtual void OnCloseAttempt()
    {
        Close();
    }
    virtual void OnUpdate()
    {
        PollEvents();
        HandleEvents();
        int wW, wH, wX, wY;
        GetSize(&wW, &wH);
        GetPos(&wX, &wY);
        if((xP + sprite.drawW) >= wW + wX && xV > 0)
        {
            xV = -xV;
            xP = wW + wX - sprite.drawW;
        }
        else if(xP <= wX && xV < 0)
        {
            xV = -xV;
            xP = wX;
        }
        if((yP + sprite.drawH) >= wH + wY && yV > 0)
        {
            yV = -yV;
            yP = wH + wY - sprite.drawH;
        }
        else if(yP <= wY && yV < 0)
        {
            yV = -yV;
            yP = wY;
        }
        xP += xV;
        yP += yV;
        sprite.drawX = xP - wX;
        sprite.drawY = yP - wY;
        sprite.Draw(this, false, false);
        Draw();
    }
};
int main()
{
    AVE::Init();
    mWindow window;

    AVE::Window::SetShareEvents(true);
    window.Open("title", 0, 0, 640, 480);
    window.texture.LoadTexture("circle_tex.png", &window);
    window.sprite.texture = &window.texture;
    window.sprite.drawW = 300;
    window.sprite.drawH = 300;
    window.sprite.sampleH = 1024;
    window.sprite.sampleW = 1024;
    window.StartMainLoop();
    AVE::DeInit();
    return 0;
}
