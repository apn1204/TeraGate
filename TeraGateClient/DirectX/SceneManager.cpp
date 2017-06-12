#include "SceneManager.h"
SceneManager::SceneManager()
{
    _currentScene = NULL;
}
SceneManager::~SceneManager()
{
}
void SceneManager::init(Keyboard* keyboard, Renderer* renderer)
{
    _keyboard = keyboard;
    _renderer = renderer;
}
void SceneManager::release()
{
    
}
void SceneManager::update()
{
    _renderer->update();
}
void SceneManager::render()
{
    _renderer->render();
}