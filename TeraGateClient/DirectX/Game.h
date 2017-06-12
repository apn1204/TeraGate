#pragma once
#include"Prev.h"
#include"SceneManager.h"
class Game{
//▼ 싱글톤 관련 
// 싱글톤쓰는 이유는 WinProc()에서 getSingletone()할라궁.
// WinProc()에서 싱글톤으로 proc()을 처리해야하는 이유는
// Game클래스를 전역변수로 쓰기 싫으니까..
// 괜히 나중에 소스코드 보다가 이거 왜 싱글톤으로 했지? 하면서 돌려놓지 말자..
private:
    Game();
    static Game* _singleton;
public:
    static Game* getSingletone();
//▼ 메소드
public:
    void init();
    void release();
    void run();
    void proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
private:
    void initWindow();
    void releaseWindow();
    void render();// run() 내부호출 함수1
    void update();// run() 내부호출 함수2
//▼ 멤버변수 
private:
    // 인풋, 그래픽
    Keyboard* _keyboard    = NULL;
    Renderer* _renderer    = NULL;
    SceneManager* _sceneManager = NULL;
    // 윈도우 관련
    LPCWSTR   _appTitle    = L"TeraGate";
    HINSTANCE _hInstance   = GetModuleHandle(NULL);
    HWND      _hwnd        = NULL;
    RECT      _rectWindow;
};
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);