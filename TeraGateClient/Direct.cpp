#include"Direct.h"
Direct::Direct()
{
    _swapchain          = 0;
    _device             = 0;
    _deviceContext      = 0;
    _renderTargetView   = 0;
    _depthStencilBuffer = 0;
    _depthStencilState  = 0;
    _depthStencilView   = 0;
    _rasterState        = 0;
}
Direct::Direct(const Direct& direct)
{

}
Direct::~Direct()
{

}
/**********************************************************************************
# ����
��ü Direct3D ������ �Ͼ�� ��

# ����
- screenWidth, screenHeight
�̹� ������ �������� �ʺ�� �����̴�.
Direct3D������ �̸� �̿��� ������ ũ���� ������ �ʱ�ȭ�ϰ� �̿��Ѵ�.

- vsync
Direct3D�������� ������� ���ΰ�ħ ������ ���� ������
�ƴϸ� ������ ������ �ٽ� �׸� ������ ����

- window
Direct3D���� �� ������ �̿��� ������ �����쿡 ����

- fullscreen
���ø����̼��� ������ ���, Ǯ��ũ�� � ��忡�� ���ư������� ����

- screenDepth, screenNear
�����쿡 �׷��� 3Dȯ�濡���� ���� ��.
**********************************************************************************/
bool Direct::init(
    int screenWidth, int screenHeight,
    bool vsync, HWND window, bool fullScreen,
    float screenDepth, float screenNear)
{
    HRESULT                       result;
    IDXGIFactory*                 factory;
    IDXGIAdapter*                 adapter;
    IDXGIOutput*                  adapterOutput;
    unsigned int                  modeNumber;
    unsigned int                  i;
    unsigned int                  numerator;      // ����� ���ļ� ����
    unsigned int                  denominator;    // ����� ���ļ� �и�
    unsigned int                  stringLength;
    DXGI_MODE_DESC*               displayModeList;
    DXGI_ADAPTER_DESC             adapterDesc;
    int                           error;
    DXGI_SWAP_CHAIN_DESC          swapchainDesc;
    D3D_FEATURE_LEVEL             featureLevel;
    ID3D11Texture2D*              backBufferPtr;
    D3D11_TEXTURE2D_DESC          depthBufferDesc;
    D3D11_DEPTH_STENCIL_DESC      depthStencilDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    D3D11_RASTERIZER_DESC         rasterDesc;
    D3D11_VIEWPORT                viewPort;
    float                         fieldOfView, screenAspect;

    /**********************************************************************************
    # �ϵ���� ����
    Direct3D�� �ʱ�ȭ�ϱ� ���� �ϵ����(�׷���ī��, ����� ���ļ�)�� ���� �˾ƾ� �Ѵ�.
    ���� �� �۾��� ���� �ʰ� ���� �����Ǵ� �⺻������ ������´ٸ�,
    DirectX�� buffer flip��� blit�� ����� ȭ�鿡 ǥ���ϰ� �Ǿ� ���ø����̼��� ������ ����߸���.
    **********************************************************************************/
    // ��������ȭ vsync ����
    _vsyncEnabled = vsync; 

    // DirectX �׷��� �������̽� ���丮�� �����.
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result))
        return false;

    // ���丮 ��ü�� ����� ù��° �׷��� ī�� �������̽��� ���� ����� ����
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result))
        return false;

    // ���(�����)�� ���� ù��° ����͸� ����
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result))
        return false;

    // DXGI_FORMAT_R8G8B8A8_UNORM ����� ��� ���÷��� ���˿� �´� ����� ��� ������ ���Ѵ�.
    result = adapterOutput->GetDisplayModeList(
        DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeNumber, NULL);

    // ������ ��� ����Ϳ� �׷���ī�� ������ ������ ����Ʈ ����
    displayModeList = new DXGI_MODE_DESC[modeNumber];
    if (!displayModeList)
        return false;

    // ���÷��� ��忡 ���� ����Ʈ ������ ä���ִ´�.
    result = adapterOutput->GetDisplayModeList(
        DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeNumber, displayModeList);
    if (FAILED(result))
        return false;

    // ��� ���÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��� ��带 ã�´�.
    // ������ ���� ã���� ������� ���ΰ�ħ ������ �и�� ���� ���� �����Ѵ�.
    for (i = 0; i < modeNumber; i++)
        if (displayModeList[i].Width  == (unsigned int)screenWidth &&
            displayModeList[i].Height == (unsigned int)screenHeight)
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;
            }

    // ������� description�� �����´�.
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result))
        return false;

    // ���� �׷���ī���� �޸� �뷮�� �ް�����Ʈ ������ �����Ѵ�.
    _videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // �׷���ī���� �̸��� char�� ���ڿ� �迭�� �ٲ� �� �����Ѵ�.
    error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0)
        return false;

    // ���÷��� ��� ����Ʈ�� �Ҵ��� �����Ѵ�.
    delete[] displayModeList;
    displayModeList = 0;

    // ����� ����� �Ҵ������Ѵ�.
    adapterOutput->Release();
    adapterOutput = 0;

    // ����͸� �Ҵ������Ѵ�.
    adapter->Release();
    adapter = 0;

    // ���丮 ��ü�� �Ҵ� �����Ѵ�.
    factory->Release();
    factory = 0;

    /**********************************************************************************
    # ����ü�� description ����ü �ʱ�ȭ - ����� ����, ������, ����
    �ý��ۿ��� ���� ���ļ��� ������ �����Ƿ� DirectX�� ����� �ʱ�ȭ�� �� �� �ִ�.
    ���� ����ü���� �ʱ�ȭ�ҰŴ�.
    ����ü���� ���� �������� �� �� �������� ���̴�.
    ����ü���� ����Ʈ���ۿ� ����۷� �̷�����ִ�.
    ���������� ����Ʈ���ۿ� ����۸� �ٲ�ġ��(swap)�ϱ� ������ ����ü���̶� �Ҹ���.
    **********************************************************************************/
    // ����ü�� description ����ü �ʱ�ȭ
    ZeroMemory(&swapchainDesc, sizeof(swapchainDesc));
    // �ϳ��� ����۸��� ���
    swapchainDesc.BufferCount = 1;
    // ������� �ʺ�, ���� ����
    swapchainDesc.BufferDesc.Width  = screenWidth;
    swapchainDesc.BufferDesc.Height = screenHeight;
    // ����۷� �Ϲ����� 32bit�� �����̽��� ����
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    /**********************************************************************************
    # ����ü�� description ����ü �ʱ�ȭ - ����� ���ļ�(���ΰ�ħ ����)
    ���ļ�(���ΰ�ħ ����)�� �ʴ� �� ���� ����۸� �׷� ����Ʈ ���ۿ� �ٲ�ġ���ϴ����� ��Ÿ���� ��ġ.
    ���� GrapihcManager�� vsync������ true�� �������ִٸ� �ý��ۿ��� ���� ���ΰ�ħ ������ ����(ex:60hz)
    �ݸ� vsync ������ false��� ���α׷��� �ִ��� ���� ȭ���� �׷������� ������, �ణ�� �ܻ��� �����.
    **********************************************************************************/
    // ������� ���ΰ�ħ ������ ����
    if (_vsyncEnabled)
    {
        swapchainDesc.BufferDesc.RefreshRate.Numerator   = numerator;
        swapchainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        swapchainDesc.BufferDesc.RefreshRate.Numerator   = 0;
        swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    /**********************************************************************************
    # ����ü�� description ����ü �ʱ�ȭ - ��Ÿ���
    **********************************************************************************/
    // ������� �뵵 ����
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // �������� �̷���� ������ �ڵ� ����
    swapchainDesc.OutputWindow = window;

    // ��Ƽ���ø��� ����.
    swapchainDesc.SampleDesc.Count   = 1;
    swapchainDesc.SampleDesc.Quality = 0;

    // ������ ��� �Ǵ� Ǯ��ũ�� ��带 ����
    if (fullScreen)
        swapchainDesc.Windowed = false;
    else
        swapchainDesc.Windowed = true;
    
    // ��ĵ������ ���İ� ��ĵ���̴��� �������� ����(unspecified) ����
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapchainDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

    // ��µ� ���� ������� ������ �������� ����
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // �߰� �ɼ� �÷��׸� ������� �ʴ´�.
    swapchainDesc.Flags = 0;

    /**********************************************************************************
    # ���ķ��� ����
    ��ó������ ��� ������ DirectX�� ������� �������ش�.
    **********************************************************************************/
    // DirectX11�� �ǹ��ϴ� 11.0���� ����
    featureLevel = D3D_FEATURE_LEVEL_11_0;

    /**********************************************************************************
    # ����̽�, ����ü�� �����
    �ϵ���� ���� �� ����ü�� description, ��ó���� ������ ���ƴ�.
    ���� ����ü��, Direct3D ����̽�, Direct3D ����̽� ���ؽ�Ʈ�� ���� �� �ִ�.

    Direct3D ����̽�, ����̽� ���ؽ�Ʈ�� ��� Direct3D����� �������̽��� �����ϱ� ������ �ſ� �߿��ϴ�.
    Direct3D ����̽�, ����̽� ���ؽ�Ʈ�� ���� ��κ��� �۾��� ������ ���̴�.

    ���� ������ DirectX�� �ͼ��� ����̸� Direct3D ����̽��� ģ���ϳ� ����̽� ���ؽ�Ʈ�� ������ �� �ִ�.
    �� ���� ���� Direct3D ����̽��� ����� �ѷ� �ɰڴٰ� �����ϸ� �ȴ�.

    ���� ������ DirectX11�� �����ϴ� �׷���ī�带 ������ ���� �ʴٸ�,
    ����̽�, ����̽� ���ؽ�Ʈ�� �����ϴ� �Լ��� ������ ���̴�.

    ���� DirectX11�� ����� �׽�Ʈ�غ��� ������ �׷���ī�尡 �������� �ʴ� ��쿡��,
    D3D_DRIVER_TYPE_HARDWARE�� D3D_DRIVER_TYPE_REFERENCE�� �ٲپ �������� CPU���� ó���ϰ� �� �� �ִ�.
    �ӵ��� 1/1000 ������ �������� DirectX11�� �������� �ʴ� �׷���ī�带 ���� ����鿡�Դ� ���� ����̴�.
    **********************************************************************************/
    // ����ü��, Direct3D ����̽�, ����̽� ���ؽ�Ʈ�� ����
    // ��Ȥ ù ��° �׷��� ī�尡 DirectX11�� ȣ�ѵ��� �ʾ� �����ϴ� ��찡 �ִ�.
    // ù ��° �׷���ī�尡 DirectX 10������ �����ϰ� �� ��° �׷���ī�忡�� DirectX11�� �����ϴ� ��Ⱑ ���� �� �ִ�.
    // ���� �Ϻ� ���̺긮�� �׷���ī���� ��� ù ��° �׷���ī��� �������� ����, �� ��° ī�尡 Nvidia�� ������ �� �ִ�.
    // �̷� ��쿡 ��� �����ϱ� ���� ���� �⺻ �׷���ī��� �������� �ʰ� ��� ���� ī����� �����Ͽ� ���� �� �´� �׷��� ī��� �����ؾ� �Ѵ�.
    result = D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
        D3D11_SDK_VERSION, &swapchainDesc, &_swapchain, &_device, NULL, &_deviceContext);
    if (FAILED(result))
        return false;

    /**********************************************************************************
    # ����� ���� �� ����ü�ΰ� ����
    **********************************************************************************/
    // ����� �����͸� �����´�.
    result = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result))
        return false;
    
    // ����� �����ͷ� ����Ÿ�� �並 ����
    result = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
    if (FAILED(result))
        return false;
    
    // ����� �����͸� �� �̻� ������� �����Ƿ� �Ҵ� ����
    backBufferPtr->Release();
    backBufferPtr = 0;

    /**********************************************************************************
    # �������� description ����ü �ۼ�
    �������� description���� ���̹��۸� ������ 3D�������� ��������� �ùٸ��� �׷�����. 
    ���ٽ� ���۵� �� �������ۿ� ������ ���̴�.
    ���ٽ� ���۴� ��� ���� ���� �ִ� �׸��� ���� ȿ���� ��Ÿ�� �� ���ȴ�.
    **********************************************************************************/
    // �������� description �ʱ�ȭ
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    
    // �������� description �ۼ�
    depthBufferDesc.Width              = screenWidth;
    depthBufferDesc.Height             = screenHeight;
    depthBufferDesc.MipLevels          = 1;
    depthBufferDesc.ArraySize          = 1;
    depthBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count   = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags     = 0;
    depthBufferDesc.MiscFlags          = 0;
    
    // description�� ����� ���������� �ؽ��� ����
    result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
    if (FAILED(result))
        return false;
    
    // ����-���ٽ� ������ description�� �ʱ�ȭ
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // ����-���ٽ� ������ description�� �ۼ�
    depthStencilDesc.DepthEnable      = true;
    depthStencilDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc        = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable    = true;
    depthStencilDesc.StencilReadMask  = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;

    // description�� ����� ����-���ٽ� ���¸� ����
    result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
    if (FAILED(result))
        return false;

    // ������ ����-���ٽ� ���¸� �����Ѵ�.
    _deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

    /**********************************************************************************
    # ����-���ٽ� ������ �� description �ۼ�
    �� �۾��� �ؾ� Direct3D���� �������۸� ����-���ٽ� �ؽ��ķ� �ν��Ѵ�. 
    �� ����ü�� ä��� �� �Ŀ� CreateDepthStencilView �Լ��� ȣ���� �����Ѵ�.
    **********************************************************************************/
    // ����-���ٽ� �� description �ʱ�ȭ
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    
    // ����-���ٽ� �� description �ۼ�
    depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // ����-���ٽ� �� ����
    result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
    if (FAILED(result))
        return false;

    // OMSetRenderTargets() �Լ� ȣ��
    // ����Ÿ�ٺ�� ����-���ٽ� �並 ��� ������ ���������ο� ���ε���Ų��.
    // �̷��� �Ͽ� ������������ �̿��� �������� ���൵�� �� ������ �������
    // ����ۿ� �׷����� �ȴ�. 
    // ����ۿ� �׷��� ���� ����Ʈ ���ۿ� �ٲ�ġ���Ͽ� ������ ����Ϳ� ������ָ� �ȴ�.
    _deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

    /**********************************************************************************
    # �����Ͷ����� ���� 
    ���� Ÿ���� ������ �� �߰����� ����� ������ �� �ִ�.
    �����Ͷ����� ���´� ������ ��� �׷����� ���� ��� �� �� �ְ� ���ش�.
    �̸� �̿��� ȭ���� ���̾������� ���� �׸��ų�, ������ �յ޸��� ��� �׸����� �� ���� �ִ�.
    �⺻������ DirectX���� �����ǰ� ���ư��� �����Ͷ����� ���°� ������
    ���� ������ �ʴ� �̻� �̿� ���� ������� ����.
    **********************************************************************************/
    // � ������ ��� �׸� ������ �����ϴ� �����Ͷ����� description �ۼ�
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // �ۼ��� description���� �����Ͷ����� ���� ����
    result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
    if (FAILED(result))
        return false;

    // �����Ͷ����� ���� ����
    _deviceContext->RSSetState(_rasterState);

    /**********************************************************************************
    # �� ��Ʈ
    �� ��Ʈ�� �־�� ����Ÿ�� �������� Ŭ������ ������ �� �ִ�.
    **********************************************************************************/
    // ������ ũ��� �����ϰ� �����Ѵ�.
    viewPort.Width = (float)screenWidth;
    viewPort.Height = (float)screenHeight;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;

    // ����Ʈ ����
    _deviceContext->RSSetViewports(1, &viewPort);

    /**********************************************************************************
    # ���� ��� projection matrix
    ��������� 3Dȭ���� �ռ� ������� 2D ����Ʈ �������� ��ȯ�ϵ��� ���ش�.
    �� ����� ���纻�� ����� ���̴����� ����� �� �ֵ��� �ؾ� �Ѵ�.
    **********************************************************************************/
    // ������� ����
    fieldOfView = (float)D3DX_PI / 4.0f;
    screenAspect = (float)screenWidth / (float)screenHeight;

    // 3D�������� ���� ��������� ����
    D3DXMatrixPerspectiveFovLH(&_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

    /**********************************************************************************
    # ���� ���
    ������Ʈ���� 3D ������ ��ǥ�� ��ȯ�ϴ� �� ���ȴ�.
    ���� 3���� �󿡼��� ȸ��/�̵�/ũ�� ��ȯ���� ���ȴ�.
    �� ���� ���纻�� ����� ���̴����� ����� �� �ְ� �Ѵ�.
    **********************************************************************************/
    // ���� ����� ���� ��ķ� �ʱ�ȭ
    D3DXMatrixIdentity(&_worldMatrix);

    /**********************************************************************************
    # �� ���
    �� ����� ���� ��鿡�� ���̸Ӱ� ��� ���� �ִ°��� ����ϴ� �� ���ȴ�.
    ���� ���ϴ� ī�޶� �� ���� �����ϸ� �ȴ�.
    **********************************************************************************/
    // �� ��� �ڵ� �ۼ��� ���߿� ī�޶� Ŭ���� Ʃ�丮�� ��..

    /**********************************************************************************
    # ���翵 ���
    3D ��ü�� �ƴ϶� UI�� ���� 2D ��ҵ��� �׸��� ���� ���ȴ�.
    **********************************************************************************/
    // ���翵 ��� ����
    D3DXMatrixOrthoLH(&_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

    return true;
}

/**********************************************************************************
init()���� ����ߴ� �����͵��� ����, �ݳ��Ѵ�.
Ǯ��ũ�� ���¿��� ����ü���� ������ �Ͼ �� ��� ���ܰ� �߻��ϱ� ������
init()���� ����ߴ� �����͸� �ݳ��ϱ� ���� ����ü���� ������ ���� �ٲ���� �Ѵ�.
**********************************************************************************/
void Direct::shut()
{
    // ������ �ݳ� ���� ������ ���� �ٲ��ش�.
    if (_swapchain)
        _swapchain->SetFullscreenState(false, NULL);

    // �Ʒ����ʹ� �� ����, �ݳ�
    if (_rasterState)
    {
        _rasterState->Release();
        _rasterState = 0;
    }
    if (_depthStencilView)
    {
        _depthStencilView->Release();
        _depthStencilView = 0;
    }
    if (_depthStencilState)
    {
        _depthStencilState->Release();
        _depthStencilState = 0;
    }
    if (_depthStencilBuffer)
    {
        _depthStencilBuffer->Release();
        _depthStencilBuffer = 0;
    }
    if (_renderTargetView)
    {
        _renderTargetView->Release();
        _renderTargetView = 0;
    }
    if (_deviceContext)
    {
        _deviceContext->Release();
        _deviceContext = 0;
    }
    if (_device)
    {
        _device->Release();
        _device = 0;
    }
    if (_swapchain)
    {
        _swapchain->Release();
        _swapchain = 0;
    }
    return;
}
/**********************************************************************************
beginScene�� ���ο� �� �����Ӹ��� 3Dȭ���� �׸��� ������ �� ȣ��ȴ�.
���۸� �� ������ �ʱ�ȭ�ϰ� �������� �̷�������� �غ��Ѵ�.
**********************************************************************************/
void Direct::beginScene(float red, float green, float blue, float alpha)
{
    // ���۸� � �������� ���� ������ ����
    float color[4] = { red, green, blue, alpha };

    // ����� ������ ����
    _deviceContext->ClearRenderTargetView(_renderTargetView, color);

    // ���� ���۸� ����
    _deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    return;
}

/**********************************************************************************
endScene�� ���ο� �� �����Ӹ��� 3Dȭ���� �׸� �� ȣ��ȴ�.
����ü�ο��� ����ۿ� �׸� 3Dȭ���� ǥ���ϵ��� �Ѵ�.
**********************************************************************************/
void Direct::endScene()
{
    // �������� �Ϸ�Ǿ����Ƿ� ������� ������ ȭ�鿡 ǥ��
    if (_vsyncEnabled)
        // ���ΰ�ħ ������ ����
        _swapchain->Present(1, 0);
    else
        // ������ ������ ǥ��
        _swapchain->Present(0, 0);

    return;
}

/**********************************************************************************
������ ��ȯ
**********************************************************************************/
ID3D11Device* Direct::getDevice()
{
    return _device;
}

/**********************************************************************************
������ ��ȯ
**********************************************************************************/
ID3D11DeviceContext* Direct::getDeviceContext()
{
    return _deviceContext;
}

/**********************************************************************************
���� ��� ��ȯ
**********************************************************************************/
void Direct::getProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
    projectionMatrix = _projectionMatrix;
    return;
}

/**********************************************************************************
���� ��� ��ȯ
**********************************************************************************/
void Direct::getWorldMatrix(D3DXMATRIX& worldMatrix)
{
    worldMatrix = _worldMatrix;
    return;
}

/**********************************************************************************
���翵 ��� ��ȯ
**********************************************************************************/
void Direct::getOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
    orthoMatrix = _orthoMatrix;
    return;
}

/**********************************************************************************
�׷���ī�� �̸�, ��밡���� �޸� �� ��ȯ
���� �ٸ� ȯ�濡�� ������ϴ� �� ������ �� �� �ִ�.
**********************************************************************************/
void Direct::getVideoCardInfo(char* cardName, int& memory)
{
    strcpy_s(cardName, 128, _videoCardDescription);
    memory = _videoCardMemory;
    return;
}