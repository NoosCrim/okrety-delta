#include "event.hpp"
#include "window.hpp"
#include <iostream>
namespace AVE
{
    bool Event::getGlobalMousePos;
    void Event::PollEvents()
    {
        SDL_Event event;
        if(!Window::shareEvents) while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_WINDOWEVENT:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
            case SDL_MOUSEMOTION:
                if(int ID = event.window.windowID)
                {
                    auto windowIter = Window::IDtoWindow.find(ID);
                    if(windowIter != Window::IDtoWindow.end())
                        windowIter->second.events.push(event);
                }
                break;
            }
        }
        else while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_WINDOWEVENT:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
            case SDL_MOUSEMOTION:
                for(std::pair<uint32_t, Window&> p : Window::IDtoWindow)
                {
                    p.second.events.push(event);
                }
                break;
            }
        }
    }
}
