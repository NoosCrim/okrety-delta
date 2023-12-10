#pragma once
#include <thread>
#include <mutex>
#include <SDL2/SDL.h>
#include <unordered_map>
#include <queue>
namespace AVE
{
    class Window
    {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        bool isOpen = false;
        std::mutex mainLoopLock;
        static std::mutex sharedMainLoopLock;
        bool shouldMainLoopStop = false;
        static bool shouldSharedMainLoopStop;
        bool pollEventsQueued = false;
        static bool sharedPollEventsQueued;
        std::queue<SDL_Event> events;
        static bool shareEvents;
        static std::unordered_map<uint32_t, Window&> IDtoWindow;
    protected:
        virtual void OnUpdate(){};
        virtual void OnCloseAttempt(){Close();};
    public:
        SDL_Window* GetWindowHandle();
        SDL_Renderer* GetRendererHandle();

        bool Open(const char* title, int x, int y, int w, int h);
        void Close();
        bool IsOpen();

        static void SetShareEvents(bool shareEvents);//set whether events should be shared between windows or not;

        void SetPos(int x, int y);
        void SetSize(int w, int h);
        void GetPos(int *x, int *y);
        void GetSize(int *w, int *h);
        void SetFullscreen();
        void SetBorderless();
        void SetWindowed();

        void QueuePollEvents();//requests event poll on the start of next frame

        void HandleEvents();

        static bool PollEvents();

        void Draw();

        void StartMainLoop();
        void StopMainLoop();
        void BlockingStopMainLoop();
        static bool StartSharedMainLoop(Window** windows, uint32_t windowCount, void (*OnSharedUpdate)());
        static void StopSharedMainLoop();
        static void BlockingStopSharedMainLoop();

    };
}

