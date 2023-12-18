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
    Window::~Window()
    {
        Close();
    }
    void Window::Update()
    {
        HandleEvents();
        if(mouseClickL)
        {
            for(Clickable* clickable : myClickables)
                if(clickable->enabled && clickable->OnClick(mouseX, mouseY, mouseClickL))
                {
                    mouseClickL = 0;
                    break;
                }
            OnClick(mouseX, mouseY, mouseClickL);
        }
        for(Active* active : myActives)
            active->OnUpdate();
        Draw();
        OnUpdate();
    }
    SDL_Window* Window::GetWindowHandle()
    {
        return window;
    }
    SDL_Renderer* Window::GetRendererHandle()
    {
        return renderer;
    }
    Texture* Window::LoadTexture(const char* path)
    {
        Texture* newTex = new Texture(path, this);
        if(!newTex->texture)
        {
            delete newTex;
            return nullptr;
        }
        return newTex;
    }
    void Window::DeleteTexture(Texture* tex)
    {
        delete tex;
    }
    bool Window::Open(const char* title, int x, int y, int w, int h)
    {
        if(isOpen)
        {
            std::cerr << "Failed to open already open window " << this << std::endl;
            return false;
        }

        if((isOpen = (Init() && (window = SDL_CreateWindow(title, x, y, w, h, 0)) && (renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))))
        {
            bgColor = {255,255,255};
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            IDtoWindow.insert({SDL_GetWindowID(window), *this});
            std::clog << "Opened window " << this << std::endl;
            return true;
        }
        std::cerr << "Failed to open window " << this << std::endl;
        return false;
    }
    void Window::Close()
    {
        StopMainLoop();
        for(std::list<Texture*>::iterator iter = myTextures.begin(); iter != myTextures.end(); )
        {
            std::list<Texture*>::iterator curr = iter++;
            delete (*curr);
        }
        for(std::list<Active*>::iterator iter = myActives.begin(); iter != myActives.end(); )
        {
            std::list<Active*>::iterator curr = iter++;
            delete (*curr);
        }
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
    void Window::GetPos(int *x, int *y)
    {
        SDL_GetWindowPosition(window, x, y);
    }
    void Window::GetSize(int *w, int *h)
    {
        SDL_GetWindowSize(window, w, h);
    }
    uint8_t Window::GetLClicks()
    {
        return mouseClickL;
    }
    void Window::GetMousePos(int* x, int* y)
    {
        (*x) = mouseX;
        (*y) = mouseY;
    }
    void Window::SetBackgroundColor(RGB color)
    {
        bgColor = color;
    }
    void Window::SetFullscreen()
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
    void Window::SetBorderless()
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    void Window::SetWindowed()
    {
        SDL_SetWindowFullscreen(window, 0);
    }
    void Window::HandleEvents()
    {
        mouseClickL = 0;
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
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT)
                    mouseClickL = event.button.clicks;
            }
        }
        if(Event::getGlobalMousePos) SDL_GetGlobalMouseState(&mouseX, &mouseY);
        else SDL_GetMouseState(&mouseX, &mouseY);
    }
    void Window::Draw()
    {
        for(Sprite* sprite : mySprites)
            sprite->Draw();
        SDL_RenderPresent(renderer);

        SDL_SetRenderDrawColor(renderer, bgColor.R, bgColor.G, bgColor.B, 255);
        SDL_RenderClear(renderer);
        return;
    }
    void Window::StartMainLoop()
    {
        if(!isOpen || !mainLoopLock.try_lock())
            return;
        shouldMainLoopStop = false;
        std::clog << "Started main window loop of window " << this << std::endl;
        OnStart();
        while(!shouldMainLoopStop)
        {
            Event::PollEvents();
            Update();
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
        for(uint32_t i = 0; i < windowCount; i++)
            if(windows[i]->IsOpen())
                windows[i]->OnStart();
        while(!shouldSharedMainLoopStop && someOpen)
        {
            someOpen = false;
            Event::PollEvents();

            for(uint32_t i = 0; i < windowCount; i++)
            {
                if(windows[i]->IsOpen())
                {
                    someOpen = true;
                    windows[i]->Update();
                }
            }
            if(OnSharedUpdate) OnSharedUpdate();
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
