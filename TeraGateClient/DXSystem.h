/*****************************************************************************************************
FileName : GameEngine.h
*****************************************************************************************************/
#pragma once
#define WIN32_LEAN_AND_MEAN // ���� ������ �ʴ� API�� ����ִ� Win32����� ������ ���� �ӵ� ���
#include<Windows.h>
#include"ObjectManager.h"
#include"Input.h"
#include"Graphic.h"
class DXSystem
{
public:
    DXSystem();
    DXSystem(const DXSystem&);
    ~DXSystem();

    bool init();
    void shut();
    void run();

    LRESULT CALLBACK procMessage(HWND, UINT, WPARAM, LPARAM);
private:
    bool frame();
    void initWindow(int&, int&);
    void shutWindow();
private:
    LPCWSTR   _applicationName;
    HINSTANCE _instance;
    HWND      _window;

    Input*   _input;
    Graphic* _graphic;
};
// ���� �Լ�
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// ���� ����
static DXSystem* gDXSystem = 0;