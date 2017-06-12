#pragma once
#include<vector>
#include"Prev.h"
#include"Keyboard.h"
#include"Renderer.h"
#include"SceneIngame.h"
#include"SceneIntro.h"
class SceneManager
{
public:
    SceneManager();
    ~SceneManager();
    void init(Keyboard*, Renderer*);
    void release();
    void update();
    void render();
private:
    Keyboard* _keyboard;
    Renderer* _renderer;

    std::vector<Scene*> _scene;
    Scene* _currentScene;
};