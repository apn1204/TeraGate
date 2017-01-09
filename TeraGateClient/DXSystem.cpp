#include"DXSystem.h"
DXSystem::DXSystem()
{
    _input   = 0;
    _graphic = 0;
}
DXSystem::DXSystem(const DXSystem& other)
{

}
DXSystem::~DXSystem()
{

}
// - ��ǲ&�׷��� ��ü �ʱ�ȭ
// - ���ø����̼��� ����� ������ ����
bool DXSystem::init()
{
    bool result;
    int  screenWidth, screenHeight;

    screenWidth = 0; screenHeight = 0;
    initWindow(screenWidth, screenHeight);

    // InputManager ��ü ����, �ʱ�ȭ
    _input = new Input;
    if (!_input)
        return false;
    _input->init();

    // GraphicManager ��ü ����, �ʱ�ȭ
    _graphic = new Graphic;
    if (!_graphic)
        return false;
    result = _graphic->init(screenWidth, screenHeight, _window);
    if (!result)
        return false;

    return true;
}
// - ��ǲ&�׷��� ��ü�� ������ ���� �ڵ��� �����Ѵ�.
void DXSystem::shut()
{
    if (_graphic)
    {
        _graphic->shut();
        delete _graphic;
        _graphic = 0;
    }
    if (_input)
    {
        delete _input;
        _input = 0;
    }
    shutWindow();
    return;
}
// - ���α׷��� ����� ������ ������ ���� ���ø����̼��� ��� �۾��� ó���Ѵ�.
// - ���ø����̼��� ��� �۾��� �� �������� �Ҹ��� frame() �Լ����� ����ȴ�.
void DXSystem::run()
{
    MSG  message;
    bool isRunning;
    // �޽��� ����ü �ʱ��
    ZeroMemory(&message, sizeof(MSG));
    // �����κ��� ���� �޽����� ���� ������ ������ ����.
    isRunning = true;
    while (isRunning)
    {
        // ������ �޽��� ó��
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        // ������ ���� �޽����� ������ �������´�.
        if (message.message == WM_QUIT)
            isRunning = false;
        // frame() �Լ� ����
        // ���� ����� true  ����
        // ���� ����� false ����, ������ �������´�.
        if (frame() == false)
            isRunning = false;
    }
    return;
}
// - ���ø����̼��� ��� �۾� ó��
bool DXSystem::frame()
{
    // Esc �Է½� ���ø����̼� ����
    if (_input->isKeyDown(VK_ESCAPE))
        return false;
    if (_input->isKeyDown(VK_RETURN)) // ���⼭ ������Ʈ �Ŵ��� ��ü �� + 1
        return false;
    // �׷��� ��ü�� �۾��� ó��
    if (_graphic->frame() == false)
        return false;
    // run() ���� ��� ����
    return true;
}
// - ������ �޽��� ó��
// - WndProc() static ���� �Լ� ������ ȣ��ȴ�.
LRESULT CALLBACK DXSystem::procMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_KEYDOWN:
        _input->keyDown((unsigned int)wParam);
        return 0;
    case WM_KEYUP:
        _input->keyUp((unsigned int)wParam);

        //        ObjectManager::getInstance()->add(new Object);
        break;
    default:
        return DefWindowProc(window, message, wParam, lParam);
    }// switch
}
// - �������� ������ ����
void DXSystem::initWindow(int& screenWidth, int& screenHeight)
{
    WNDCLASSEX windowClass;
    DEVMODE    screenSetting;
    int        posX, posY;

    // �ܺ� �����͸� �� ��ü�� ����
    gDXSystem = this;

    // �� ���ø����̼��� �ν��Ͻ��� �����´�.
    _instance = GetModuleHandle(NULL);
    // ���ø����̼��� �̸��� ����
    _applicationName = L"GameEngine";
    // ������ Ŭ���� �ʱ�ȭ, ���
    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = _instance;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hIconSm = windowClass.hIcon;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = _applicationName;
    windowClass.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&windowClass);
    // ����� ȭ���� �ػ󵵸� �˾ƿ´�.
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    if (FULL_SCREEN)
    {
        // Ǯ��ũ�� ����� ȭ�� ũ�⸦ ����ũ�� ũ�⿡ ���߰� ������ 32bit��
        memset(&screenSetting, 0, sizeof(screenSetting));
        screenSetting.dmSize = sizeof(screenSetting);
        screenSetting.dmPelsWidth = (unsigned long)screenWidth;
        screenSetting.dmPelsHeight = (unsigned long)screenHeight;
        screenSetting.dmBitsPerPel = 32;
        screenSetting.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        // ��ũ���� �´� ���÷��� ����
        ChangeDisplaySettings(&screenSetting, CDS_FULLSCREEN);
        // ������ ��ġ�� ȭ�� ���� ���� �����.
        posX = posY = 0;
    }
    else
    {
        // ������ ����� 800x600 ũ��� ���߰� ȭ�� �߾ӿ� ������ �Ѵ�.
        screenWidth  = 800;
        screenHeight = 600;
        posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
    }
    // ������ ��� ������ â�� ����� �ڵ��� �����´�.
    _window = CreateWindowEx(WS_EX_APPWINDOW,
        _applicationName, _applicationName,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        posX, posY, screenWidth, screenHeight, NULL, NULL, _instance, NULL);
    // �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �ش�.
    ShowWindow(_window, SW_SHOW);
    SetForegroundWindow(_window);
    SetFocus(_window);
    // ���콺 Ŀ���� ǥ������ �ʴ´�.
    ShowCursor(false);
    return;
}
// - ȭ�� ������ �ǵ����� ������&�ڵ��� ��ȯ
void DXSystem::shutWindow()
{
    // ���콺 Ŀ���� ǥ��
    ShowCursor(true);
    // Ǯ��ũ�� ��带 �������� �� ���÷��� ������ �ٲ۴�.
    if (FULL_SCREEN)
        ChangeDisplaySettings(NULL, 0);
    // â�� �����Ѵ�.
    DestroyWindow(_window);
    _window = NULL;
    // �� Ŭ������ ���� �ܺ� ������ ������ �����Ѵ�.
    gDXSystem = NULL;
    return;
}
// - ������ �޽��� ó��
// - GameEngine::procMessage() �Լ��� ȣ���Ѵ�.
LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // �����찡 ���ŵǾ����� Ȯ��
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        // �����찡 �������� Ȯ��
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    default:
        return gDXSystem->procMessage(window, message, wParam, lParam);
    }
}