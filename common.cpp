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
    Active::~Active()
    {
        owner->UnbindActive(this);
    }
}
