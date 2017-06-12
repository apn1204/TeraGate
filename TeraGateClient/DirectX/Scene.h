#pragma once
#include"Prev.h"
class Scene
{
public:
    Scene();
    ~Scene();
    virtual void init()                           = 0;
    virtual void release()                        = 0;
    virtual void proc(HWND, UINT, WPARAM, LPARAM) = 0;
    virtual void update()                         = 0;
    virtual void render()                         = 0;
};

