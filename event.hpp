#pragma once
namespace AVE
{
    class Event
    {
        Event() = delete;
    public:
        static bool getGlobalMousePos;
        static void PollEvents();
    };
}
