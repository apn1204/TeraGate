#include"Game.h"
// 싱글톤 
Game* Game::_singleton = NULL;
Game* Game::getSingletone()
{
    if(_singleton == NULL)
        _singleton = new Game;
    return _singleton;
}
Game::Game(){}
void Game::init()
{
    initWindow();

    _keyboard     = new Keyboard;
    _renderer     = new Renderer;
    _sceneManager = new SceneManager;
    _renderer->init(_hwnd, _rectWindow.right, _rectWindow.bottom);
    _sceneManager->init(_keyboard, _renderer);

    PlaySound(L"Resource/Sound/bgmUntitled", NULL, SND_ASYNC | SND_LOOP);
#ifdef _DEBUG
    printf("Game::init()\n");
#endif
}
void Game::initWindow()
{

    WNDCLASSEX   window;
    DEVMODE      devmode;

    // 윈도우 클래스 초기화 후 등록
    window.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window.lpfnWndProc   = WinProc; // 이 부분 때문에 WinProc을 Game.cpp/h에 선언.. 영 맘에 안드는 부분.
    window.cbClsExtra    = 0;
    window.cbWndExtra    = 0;
    window.hInstance     = _hInstance;
    window.hIcon         = (HICON)LoadImage(NULL, TEXT("Resource/Windows/Venom.ico"), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    window.hIconSm       = window.hIcon;
    window.hCursor       = (HICON)LoadImage(NULL, TEXT("Resource/Windows/Venom.ani"), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    window.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    window.lpszMenuName  = NULL;
    window.lpszClassName = _appTitle;
    window.cbSize        = sizeof(WNDCLASSEX);
    RegisterClassEx(&window);

    _rectWindow = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    if(FULL_SCREEN)
    {
        //윈도영역
        _rectWindow.left   = 0; 
        _rectWindow.top    = 0; 
        _rectWindow.right  = GetSystemMetrics(SM_CXSCREEN);
        _rectWindow.bottom = GetSystemMetrics(SM_CYSCREEN);
        //devmode설정
        memset(&devmode, 0, sizeof(devmode));
        devmode.dmSize       = sizeof(devmode);
        devmode.dmPelsWidth  = GetSystemMetrics(SM_CXSCREEN);
        devmode.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);
        devmode.dmBitsPerPel = 32;
        devmode.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
        //devmode값에 따라 풀스크린 모드로
        ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
        //윈도생성
        _hwnd = CreateWindowEx(
            WS_EX_APPWINDOW,
            _appTitle, _appTitle,
            WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_POPUP,
            _rectWindow.left, _rectWindow.top, _rectWindow.right, _rectWindow.bottom,
            NULL, NULL, _hInstance, NULL);
    }
    else
    {
        //윈도영역
        _rectWindow.left   = (GetSystemMetrics(SM_CXSCREEN) - CLIENT_WIDTH)  / 2; 
        _rectWindow.top    = (GetSystemMetrics(SM_CYSCREEN) - CLIENT_HEIGHT) / 2; 
        _rectWindow.right  = CLIENT_WIDTH;
        _rectWindow.bottom = CLIENT_HEIGHT;
        _hwnd = CreateWindowEx(WS_EX_APPWINDOW,
            _appTitle, _appTitle,
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
            _rectWindow.left, _rectWindow.top,
            _rectWindow.right, _rectWindow.bottom,
            NULL, NULL, _hInstance, NULL);
        
        // 타이틀바 있게
        //_hwnd = CreateWindowEx(
        //    WS_EX_APPWINDOW,
        //    _appTitle, _appTitle,
        //    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        //    _rectWindow.left, _rectWindow.top,
        //    _rectWindow.right, _rectWindow.bottom,
        //    NULL, NULL, _hInstance, NULL);        
        
    }
    // 윈도우 전위 포커스
    ShowWindow(_hwnd, SW_SHOW);
    SetForegroundWindow(_hwnd);
    SetFocus(_hwnd);
    ShowCursor(true);
}
void Game::release()
{
    if(_keyboard)
        delete _keyboard;
    _keyboard = NULL;

    if(_renderer)
        delete _renderer;
    _renderer = NULL;

    if(_sceneManager)
        delete _sceneManager;
    _sceneManager = NULL;

    releaseWindow();
}
void Game::releaseWindow()
{
    ShowCursor(true);
    if (FULL_SCREEN)
        ChangeDisplaySettings(NULL, 0);
    DestroyWindow(_hwnd);
    UnregisterClass(_appTitle, _hInstance);
}
void Game::run()
{
    MSG  msg;
    bool isRunning = true;
    ZeroMemory(&msg, sizeof(MSG));
    while (isRunning)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT)
            isRunning = false;
        render();
        update();
    }
}
void Game::render()
{
    _sceneManager->render();
}
void Game::update()
{
    _sceneManager->update();
}
void Game::proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        _keyboard->down(wparam);
        break;
    case WM_KEYUP:
        _keyboard->up(wparam);
        break;
    case WM_PAINT:
        /*
            PAINTSTRUCT ps;
            HDC hdc;
            hdc = BeginPaint(hwnd, &ps);
            Rectangle(hdc, 0, 0, 100, 100);
            EndPaint(hwnd, &ps);
        */
        break;
    default:
        break;
    }
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    Game::getSingletone()->proc(hwnd, message, wparam, lparam);
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }
}