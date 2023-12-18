#pragma once
#include <list>
#include <cstdint>
namespace AVE
{
    class Window;
    class Active // Interface with Update function, called each frame of the owner window
    {
        friend class Window;
        Window* owner = nullptr;
        std::list<Active*>::iterator iter;
        bool started = false;
        void Update();
        void BindWindow(Window* window);
        void UnbindWindow();
    protected:
        virtual void OnStart()=0;
        virtual void OnUpdate()=0;
        Active(Window* window); // protected because it should never be created non-dynamically.
        virtual ~Active();
    public:
        Active(const Active&) = delete;
        Active(Active&) = delete;
        Window* GetWindow() { return owner; }
    };
    class Clickable
    {
        friend class Window;
        Window* owner;
        std::list<Clickable*>::iterator iter;
        bool enabled = true;
        void BindWindow(Window* window);
        void UnbindWindow();
    protected:
        virtual ~Clickable();
        virtual bool OnClick(int mX, int mY, uint8_t clicks) = 0; // returns true if no clicks under it should be checked
        Clickable(Window* window);
    public:
        Clickable(const Clickable&) = delete;
        Clickable(Clickable&) = delete;
    };
    struct RGB
    {
        uint8_t R = 0,G = 0,B = 0;
    };
}
