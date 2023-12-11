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
    protected:
        virtual void OnStart(){};
        virtual void OnUpdate(){};
        Active(){}; // protected because it should never be created non-dynamically. It and it's children should always be created using new, and not used unitl bound to Window object
    public:
        virtual ~Active();
    };
    struct RGB
    {
        uint8_t R = 0,G = 0,B = 0;
    };
}
