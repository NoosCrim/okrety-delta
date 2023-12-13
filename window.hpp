#pragma once
#include <mutex>
#include <SDL2/SDL.h>
#include <unordered_map>
#include <queue>
#include <list>
#include "common.hpp"
#include "event.hpp"
namespace AVE
{
    class Texture;
    class Sprite;
    class Window
    {
        friend void Active::BindWindow(Window* window);
        friend void Active::UnbindWindow();
        friend void Clickable::BindWindow(Window* window);
        friend void Clickable::UnbindWindow();
        friend class Texture;
        friend class Event;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        bool isOpen = false;
        std::mutex mainLoopLock;
        static std::mutex sharedMainLoopLock;
        bool shouldMainLoopStop = false;
        static bool shouldSharedMainLoopStop;
        std::queue<SDL_Event> events;
        static bool shareEvents;
        uint8_t mouseClickL;
        static std::unordered_map<uint32_t, Window&> IDtoWindow;
        std::list <Texture*> myTextures;
        std::list <Active*> myActives;
        std::list <Sprite*> mySprites;
        std::list <Clickable*> myClickables;
        RGB bgColor;
        int mouseX = 0, mouseY = 0;
        void Update();
        void _OnClick();
    protected:
        virtual void OnStart(){};
        virtual void OnUpdate(){};
        virtual void OnCloseAttempt(){Close();};
        virtual void OnClick(int mX, int mY, uint8_t clicks){};
    public:
        ~Window();

        SDL_Window* GetWindowHandle();
        SDL_Renderer* GetRendererHandle();

        Texture* LoadTexture(const char* path);
        void DeleteTexture(Texture* tex);

        bool Open(const char* title, int x, int y, int w, int h);
        void Close();
        bool IsOpen();

        static void SetShareEvents(bool shareEvents);//set whether events should be shared between windows or not;

        void SetPos(int x, int y);
        void SetSize(int w, int h);
        void GetPos(int *x, int *y);
        void GetSize(int *w, int *h);

        uint8_t GetLClicks();//returns 1 for click, 2 for double click etc.
        void GetMousePos(int* x, int* y);//writes mouse coordinates to given addresses

        void SetBackgroundColor(RGB color);

        void SetFullscreen();
        void SetBorderless();
        void SetWindowed();

        void HandleEvents(); //Processes each event in window's event queue
        void FlushEvents(); //Flushes window's event queue without handling


        void Draw();

        void StartMainLoop();
        void StopMainLoop();
        void BlockingStopMainLoop();
        static bool StartSharedMainLoop(Window** windows, uint32_t windowCount, void (*OnSharedUpdate)());
        static void StopSharedMainLoop();
        static void BlockingStopSharedMainLoop();

    };
}

