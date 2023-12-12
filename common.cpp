#include "common.hpp"
#include "window.hpp"
#include <iostream>
namespace AVE
{
    void Active::Update()
    {
        if(!started)
        {
            started = true;
            OnStart();
        }
        OnUpdate();
    }
    Active::Active(Window* window)
    {
        BindWindow(window);
    }
    Active::~Active()
    {
        UnbindWindow();
    }
    void Active::BindWindow(Window* window)
    {
        owner = window;
        iter = window->myActives.insert(window->myActives.begin(), this);
    }
    void Active::UnbindWindow()
    {
        owner->myActives.erase(iter);
        owner = nullptr;
    }
    // Clickable should always be created using new, and then bound to Window so it can be destroyed later
    Clickable::Clickable(Window* window)
    {
        BindWindow(window);
    }
    Clickable::~Clickable()
    {
        UnbindWindow();
    }
    void Clickable::BindWindow(Window* window)
    {
        owner = window;
        iter = window->myClickables.insert(window->myClickables.begin(), this);
    }
    void Clickable::UnbindWindow()
    {
        owner->myClickables.erase(iter);
        owner = nullptr;
    }
}
