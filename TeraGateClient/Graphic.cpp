/*****************************************************************************************************
FileName : Graphic.cpp
*****************************************************************************************************/
#include"Graphic.h"
Graphic::Graphic()
{
    _direct      = 0;
    _camera      = 0;
    _model       = 0;
    _colorShader = 0;
}
Graphic::Graphic(const Graphic& graphicManager)
{

}
Graphic::~Graphic()
{

}
bool Graphic::init(int screenWidth, int screenHeight, HWND window)
{
    bool result;

    /*****************************************************************************************************
    Direct ����, �ʱ�ȭ
    *****************************************************************************************************/
    _direct = new Direct;
    if (!_direct)
        return false;
    result = _direct->init(
        screenWidth, screenHeight,
        VSYNC_ENABLED, window,
        FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(window, L"Direct3D�� �ʱ�ȭ���� ���߽��ϴ�.", L"Error", MB_OK);
        return false;
    }
    /*****************************************************************************************************
    Camera ����, �ʱ�ȭ
    *****************************************************************************************************/
    _camera = new Camera;
    if (!_camera)
        return false;
    _camera->setPosition(0.0f, 0.0f, -10.0f);
    /*****************************************************************************************************
    Model ������Ʈ ����, �ʱ�ȭ
    *****************************************************************************************************/
    _model = new Model;
    if (!_model)
    {
        MessageBox(window, L"Model ��ü�� �������� ���߽��ϴ�.", L"Error", MB_OK);
        return false;
    }
    result = _model->init(_direct->getDevice());
    if (!result)
    {
        MessageBox(window, L"Model ��ü�� �ʱ�ȭ���� ���߽��ϴ�.", L"Error", MB_OK);
        return false;
    }
    /*****************************************************************************************************
    Shader ����, �ʱ�ȭ
    *****************************************************************************************************/
    _colorShader = new ColorShader;
    if (!_colorShader)
    {
        MessageBox(window, L"ColorShader ��ü�� �������� ���߽��ϴ�.", L"Error", MB_OK);
        return false;
    }
    result = _colorShader->init(_direct->getDevice(), window);
    if (!result)
    {
        MessageBox(window, L"ColorShadaer ��ü�� �ʱ�ȭ���� ���߽��ϴ�.", L"Error", MB_OK);
        return false;
    }
    // ��� �ʱ�ȭ ����
    return true;
}
void Graphic::shut()
{
    if (_colorShader)
    {
        _colorShader->shut();
        delete _colorShader;
        _colorShader = 0;
    }
    if (_model)
    {
        _model->shut();
        delete _model;
        _model = 0;
    }
    if (_camera)
    {
        delete _camera;
        _camera = 0;
    }
    if (_direct)
    {
        _direct->shut();
        delete _direct;
        _direct = 0;
    }
    return;
}
bool Graphic::frame()
{
    bool result;
    result = render();
    if (!result)
        return false;
    return true;
}
bool Graphic::render()
{
    // ���� �׸��� ���� ������ ������ �����.
    _direct->beginScene(0.0f, 0.0f, 0.5f, 1.0f);
    // �� ���� �ڵ�
    // ���� & �� & ���� ���
    D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
    bool result;
    // ī�޶� ��ġ�� ����� �� ��� ����
    _camera->render();
    
    // ī�޶� & ���̷�Ʈ ��ü�κ��� ���� & �� & ���� ��� ���
    _camera->getViewMatrix(viewMatrix);
    _direct->getWorldMatrix(worldMatrix);
    _direct->getProjectionMatrix(projectionMatrix);

    // ���� ����&�ε��� ���۸� �׷��� ���������ο� �־� ������ �غ��Ų��.
    _model->render(_direct->getDeviceContext());

    // �÷� ���̴��� �̿��� �� ����
    result = _colorShader->render(
        _direct->getDeviceContext(),
        _model->getIndexCount(),
        worldMatrix, viewMatrix, projectionMatrix);
    if (!result)
        return false;
    // �� ���� �ڵ�
    // ���ۿ� �׷��� ���� ȭ�鿡 ǥ���Ѵ�.
    _direct->endScene();
    return true;
}