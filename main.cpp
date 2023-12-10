#include <iostream>
#include "AVE.hpp"
#include <SDL2/SDL.h>
using namespace std;
class mWindow : public AVE::Window
{
    virtual void OnCloseAttempt()
    {
        Close();
    }
    virtual void OnUpdate()
    {
        PollEvents();
        HandleEvents();
        Draw();
    }
};
int main()
{
    AVE::Init();
    mWindow window;
    AVE::Window::SetShareEvents(true);
    window.Open("title", 0, 0, 640, 480);
    window.StartMainLoop();
    AVE::DeInit();
    return 0;
}
