#pragma once
#include "Scene.h"
class SceneIntro : public Scene
{
public:
    SceneIntro();
    ~SceneIntro();
    void init();
    void release();
    void proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    void update();
    void render();
};

