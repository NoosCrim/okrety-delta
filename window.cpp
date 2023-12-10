#include <iostream>
#include <mutex>
#include "window.hpp"
#include "AVE.hpp"
namespace AVE
{
    bool Window::shareEvents = false;
    std::unordered_map<uint32_t, Window&> Window::IDtoWindow;
    std::mutex Window::sharedMainLoopLock;
    bool Window::shouldSharedMainLoopStop = false;
    bool Window::sharedPollEventsQueued;
    bool Window::Open(const char* title, int x, int y, int w, int h)
    {
        if(isOpen)
        {
            std::cerr << "Failed to open already open window " << this << std::endl;
            return false;
        }

        if((isOpen = (Init() && (window = SDL_CreateWindow(title, x, y, w, h, 0)) && (renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))))
        {
            IDtoWindow.insert({SDL_GetWindowID(window), *this});
            std::clog << "Opened window " << this << std::endl;
            return true;
        }
        std::cerr << "Failed to open window " << this << std::endl;
        return false;
    }
    void Window::Close()
    {
        shouldMainLoopStop = true;
        IDtoWindow.erase(SDL_GetWindowID(window));
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        isOpen = false;
        std::clog << "Closed window " << this << std::endl;
    }
    bool Window::IsOpen()
    {
        return isOpen;
    }
    void Window::SetShareEvents(bool _shareEvents)
    {
        shareEvents = _shareEvents;
    }
    void Window::SetPos(int x, int y)
    {
        SDL_SetWindowPosition(window, x, y);
    }
    void Window::SetSize(int w, int h)
    {
        SDL_SetWindowSize(window, w, h);
    }
    void Window::HandleEvents()
    {
        while(!events.empty())
        {
            SDL_Event event = events.back();
            events.pop();
            switch(event.type)
            {
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    OnCloseAttempt();
                }
                break;
                ///TODO - Keyboard and mouse state storing
            }
        }

    }
    bool Window::PollEvents()
    {
        SDL_Event event;
        if(!shareEvents) while(SDL_PollEvent(&event))
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
                    auto windowIter = IDtoWindow.find(ID);
                    if(windowIter != IDtoWindow.end())
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
                for(std::pair<uint32_t, Window&> p : IDtoWindow)
                {
                    p.second.events.push(event);
                }
                break;
            }
        }
        return true;
    }
    void Window::Draw() ///TODO - rendering
    {
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
        return;
    }
    void Window::StartMainLoop()
    {
        if(!isOpen || !mainLoopLock.try_lock())
            return;
        shouldMainLoopStop = false;
        std::clog << "Started main window loop of window " << this << std::endl;
        while(!shouldMainLoopStop)
        {
            OnUpdate();
        }
        mainLoopLock.unlock();
        std::clog << "Stopped main window loop of window " << this << std::endl;
    }
    void Window::StopMainLoop()
    {
        shouldMainLoopStop = true;
    }
    void Window::BlockingStopMainLoop()
    {
        shouldMainLoopStop = true;
        mainLoopLock.lock();
        mainLoopLock.unlock();
    }
    bool Window::StartSharedMainLoop(Window** windows, uint32_t windowCount, void (*OnSharedUpdate)())
    {
        if(!sharedMainLoopLock.try_lock() || !windowCount)
            return false;
        for(uint32_t i = 0; i < windowCount; i++)
            if(!windows[i]->mainLoopLock.try_lock())
            {
                std::cerr << "Failed to start shared main window loop: window " << windows[i] << " already runs his main loop.";
                for(unsigned int j = i-1; j >= 0; j--)
                    windows[j]->mainLoopLock.unlock();
                return false;
            }

        std::clog << "Started shared main window loop for windows: " << windows[0];
        for(uint32_t i = 1; i < windowCount; i++)
            std::clog << ", " << windows[i];
        std::clog << std::endl;
        bool someOpen = true;
        if(OnSharedUpdate) while(!shouldSharedMainLoopStop && someOpen)
        {
            someOpen = false;
            OnSharedUpdate();
            for(uint32_t i = 0; i < windowCount; i++)
            {
                if(windows[i]->IsOpen())
                {
                    someOpen = true;
                    windows[i]->OnUpdate();
                }
            }
        }
        else while(!shouldSharedMainLoopStop && someOpen)
        {
            someOpen = false;
            for(uint32_t i = 0; i < windowCount; i++)
                if(windows[i]->IsOpen())
                {
                    someOpen = true;
                    windows[i]->OnUpdate();
                }
        }

        for(uint32_t i = 0; i < windowCount; i++)
            windows[i]->mainLoopLock.unlock();
        sharedMainLoopLock.unlock();
        std::clog << "Stopped shared main window loop for windows: " << windows[0];
        for(uint32_t i = 1; i < windowCount; i++)
            std::clog << ", " << windows[i];
        std::clog << std::endl;
        return true;
    }
    void Window::StopSharedMainLoop()
    {
        shouldSharedMainLoopStop = true;
    }
    void Window::BlockingStopSharedMainLoop()
    {
        if(sharedMainLoopLock.try_lock())
        {
            sharedMainLoopLock.unlock();
            return;
        }
        shouldSharedMainLoopStop = true;
        sharedMainLoopLock.lock();
        sharedMainLoopLock.unlock();
    }
}
