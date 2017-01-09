#include"ColorShader.h"
ColorShader::ColorShader()
{
    _vertexShader = 0;
    _pixelShader  = 0;
    _layout       = 0;
    _matrixBuffer = 0;
}
ColorShader::ColorShader(const ColorShader& other)
{

}
ColorShader::~ColorShader()
{

}

/*****************************************************************************************************
init() �Լ��� ���̴��� �ʱ�ȭ�� �����ϴ� �Լ��� ȣ���Ѵ�.
color.vs, color.ps�� HLSL ���̴� ������ �̸��� �Ѱ��ش�.
*****************************************************************************************************/
bool ColorShader::init(ID3D11Device* device, HWND window)
{
    bool result;
    result = initShader(device, window, L"Color.vs", L"Color.ps");
    if (!result)
    {
        MessageBox(window, L"���̴� �ʱ�ȭ ����", L"Error", MB_OK);
        return false;
    }

    return true;
}
// ����&�ȼ� ���̴� �� �׿� ���õ� �͵��� ��ȯ
void ColorShader::shut()
{
    shutShader();
    return;
}
bool ColorShader::render(
    ID3D11DeviceContext* deviceContext,
    int                  indexCount,
    D3DXMATRIX           worldMatrix,
    D3DXMATRIX           viewMatrix,
    D3DXMATRIX           projectionMatrix)
{
    bool result;

    // �������� ����� ���̴��� ���ڸ� �Է�
    result = setShaderParameter(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
    if (!result)
        return false;

    // ���̴��� �̿��Ͽ� �غ�� ���۸� �׸���.
    renderShader(deviceContext, indexCount);

    return true;
}
/*****************************************************************************************************
initShader() �Լ� ������ ������ ���̴� ������ �ҷ�����
DirectX�� GPU���� ��밡���ϵ��� �ϴ� ���� �Ѵ�.
��� ���� ������ �����Ͱ� GPU���� ��� ���Ǵ��� �� �� �ִ�.
���� DirectX�� GPU���� ��밡���ϵ��� �Ѵ�.
�� ���̾ƿ��� Model Ŭ���� �� �ƴ϶� color.vs�� ����� VertexType�� ��ġ�� �ʿ䰡 �ִ�.
*****************************************************************************************************/
bool ColorShader::initShader(ID3D11Device* device, HWND window, WCHAR* vsFileName, WCHAR* psFileName)
{
    HRESULT                  result;
    ID3D10Blob*              errorMessage;
    ID3D10Blob*              vertexShaderBuffer;
    ID3D10Blob*              pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int             numElements;
    D3D11_BUFFER_DESC        matrixBufferDesc;

    // �� �Լ����� ����ϴ� �����͵��� null�� �����Ѵ�.
    errorMessage       = 0;
    vertexShaderBuffer = 0;
    pixelShaderBuffer  = 0;

    /*****************************************************************************************************
    ���̴� ���α׷��� ���۷� �������Ѵ�.
    ���̴� ���� �̸�, ���̴� �̸�, ���̴��� ����(DirectX11������ 5.0), ���̴��� �����ϵ� ���۸� ���ڷ� �Ѱ��ش�.
    ���� �������� �����ϸ� ���� �޽����� errorMessage���ڿ��� ����ϰ� �̰��� �ٸ� �Լ��� �� �� �ִ�.
    ���� errorMessage���� �����Ѵٸ� ���̴� ������ ã�� ���ߴٴ� ���̴�.
    *****************************************************************************************************/
    // ���� ���̴��� ������
    result = D3DX11CompileFromFile(
        vsFileName, NULL, NULL,
        "ColorVertexShader", "vs_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0, NULL,
        &vertexShaderBuffer,
        &errorMessage, NULL);
    if (FAILED(result))
    {
        // ���̴��� �����Ͽ� �����ϸ� ���� �޽����� ���
        if (errorMessage)
            outputShaderErrorMessage(errorMessage, window, vsFileName);
        // ���� �޽����� ���ٸ� ���̴� ������ ã�� ���� ��
        else
            MessageBox(window, vsFileName, L"Missing Shader File", MB_OK);
        return false;
    }
    // �ȼ� ���̴��� ������
    result = D3DX11CompileFromFile(
        psFileName, NULL, NULL,
        "ColorPixelShader", "ps_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0, NULL,
        &pixelShaderBuffer,
        &errorMessage, NULL);
    if (FAILED(result))
    {
        // ���̴� �������� �����ϸ� ���� �޽����� ���
        if (errorMessage)
            outputShaderErrorMessage(errorMessage, window, psFileName);
        // ���� �޽����� ���ٸ� ���̴� ������ ã�� ���� ��
        else
            MessageBox(window, psFileName, L"Missing Shader File", MB_OK);
        return false;
    }

    /*****************************************************************************************************
    ����&�ȼ� ���̴��� ���۷� �� �����ϵǸ� �̸� �̿��� ���̴� ��ü�� ���� �� �ִ�.
    ���⼭ ���� �����͸� ���� ���̴��� �ȼ� ���̴��� �������̽��μ� ����� ��.
    *****************************************************************************************************/
    // ���۷κ��� ���� ���̴��� ����
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
    if (FAILED(result))
        return false;
    // ���۷κ��� �ȼ� ���̴��� ����
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
    if (FAILED(result))
        return false;

    /*****************************************************************************************************
    ���̴����� ����� ���� �������� ���̾ƿ� ����.
    ���� ���̴������� ��ġ&���� ���͸� ����ϹǷ� ���̾ƿ��� ������ ������ ũ�⸦ �����ϴ� �� ���̾ƿ��� �����.
    POSITION�� ����, COLOR�� �� ��°�� ó���Ѵ�.
    ��ġ ���ʹ� DXGI_FORMAT_R32G32B32_FLOAT    ���
    ���� ���ʹ� DXGI_FORMAT_R32G32B32A32_FLOAT ���

    ���ۿ� �����Ͱ� ��� �迭�Ǵ��� �˷��ִ� AlignedByteOffset�� ó�� 12byte�� ��ġ ���Ϳ� ����ϰ�
    ���� 16byte�� �������� ����� ������ �˷���� �ϴµ�, �� ��Ұ� ��� ���۵Ǵ����� ��Ÿ����.
    ���� ���� �Է��ϱ⺸�� D3D11_APPEND_ALIGNED_ELEMENT�� �����ϸ� �ڵ����� �˾Ƴ��� �ȴ�.
    *****************************************************************************************************/
    // ���� �Է� ���̾ƿ� description �ۼ�
    // Model Ŭ������ ���̴��� �ִ� VertexType�� ��ġ�ؾ� �Ѵ�.
    polygonLayout[0].SemanticName           = "POSITION";
    polygonLayout[0].SemanticIndex          = 0;
    polygonLayout[0].Format                 = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot              = 0;
    polygonLayout[0].AlignedByteOffset      = 0;
    polygonLayout[0].InputSlotClass         = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate   = 0;

    polygonLayout[1].SemanticName           = "COLOR";
    polygonLayout[1].SemanticIndex          = 0;
    polygonLayout[1].Format                 = DXGI_FORMAT_R32G32B32A32_FLOAT;
    polygonLayout[1].InputSlot              = 0;
    polygonLayout[1].AlignedByteOffset      = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass         = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate   = 0;

    // ���̾ƿ��� ��� ������ �����´�.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // ���� �Է� ���̾ƿ��� ����
    result = device->CreateInputLayout(
        polygonLayout,
        numElements,
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        &_layout);
    if (FAILED(result))
    {
        return false;
    }

    // �� �̻� ������ �ʴ� ���� ���̴� �۹��� �ȼ� ���̴� ���۸� ����.
    vertexShaderBuffer->Release();
    vertexShaderBuffer = 0;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = 0;

    /*****************************************************************************************************
    ���̴��� ����ϱ� ���� ������ �ܰ�, �������(constant buffer)
    �� �����Ӹ��� ������۸� ������Ʈ�ϱ� ������ ������ ����� ������ �� �ʿ䰡 �ִ�.
    BindFlags�� �̿��� ������۷� �� ���۸� ����Ѵٰ� �����Ѵ�.
    CPUAccessFlgas�� �뵵�� ���߾�� �ϱ� ������ D3D11_CPU_ACCESS_WRITE�� �����Ǿ� �Ѵ�.
    �� description�� ä������ ��������� �ζ��̽��� �����
    �̿� SetShaderParameters �Լ��� �̿��� ���̴��� ���� �����鿡 ������ �� �ֵ��� �Ѵ�.
    *****************************************************************************************************/
    // ���� ���̴��� �ִ� ��� ��� ������ description�� �ۼ�
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� ������ �� �ְ� �ȴ�
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
    if (FAILED(result))
         return false;
 
    return true;
}
/*****************************************************************************************************
initShader()���� �����ߴ� 4���� �������̽��� �����Ѵ�.
4���� �������̽� : �������, ���̾ƿ�, �ȼ����̴�, �������̴�
*****************************************************************************************************/
void ColorShader::shutShader()
{
    // ��� ���� ����
    if (_matrixBuffer)
    {
        _matrixBuffer->Release();
        _matrixBuffer = 0;
    }

    // ���̾ƿ� ����
    if (_layout)
    {
        _layout->Release();
        _layout = 0;
    }

    // �ȼ� ���̴� ����
    if (_pixelShader)
    {
        _pixelShader->Release();
        _pixelShader = 0;
    }

    // ���� ���̴� ����
    if (_vertexShader)
    {
        _vertexShader->Release();
        _vertexShader = 0;
    }

    return;
}
/*****************************************************************************************************
����&�ȼ� ���̴��� ������ ���� �����߻��� �����޽����� ���
*****************************************************************************************************/
void ColorShader::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND window, WCHAR* shaderFileName)
{
    char*         compileErrors;
    unsigned long bufferSize, i;
    ofstream      fout;


    // ���� �޼����� ��� �ִ� ���ڿ� ������ �����͸� �����´�
    compileErrors = (char*)(errorMessage->GetBufferPointer());

    // �޼��� ���̸� �����´�.
    bufferSize = errorMessage->GetBufferSize();

    // ������ ���� �ȿ� �޼����� ���
    fout.open("shader-error.txt");

    // ���� �޼����� �ۼ�
    for (i = 0; i<bufferSize; i++)
    {
        fout << compileErrors[i];
    }

    // ������ �ݴ´�.
    fout.close();

    // ���� �޼����� ��ȯ
    errorMessage->Release();
    errorMessage = 0;

    // ������ ������ ������ �˾� �޼����� �˸���.
    MessageBox(window, L"Error compiling shader.  Check shader-error.txt for message.", shaderFileName, MB_OK);

    return;
}
/*****************************************************************************************************
���̴��� ���������� ���� �ٷ�� ���� ���
*****************************************************************************************************/
bool ColorShader::setShaderParameter(
    ID3D11DeviceContext* deviceContext,
    D3DXMATRIX           worldMatrix,
    D3DXMATRIX           viewMatrix,
    D3DXMATRIX           projectionMatrix)
{
    HRESULT                  result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType*        dataPtr;
    unsigned int             bufferNumber;


    // ����� transpose�Ͽ� ���̴����� ����� �� �ְ� �Ѵ�.
    D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
    D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
    D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

    // ��� ������ ������ �� �� �ֵ��� ���
    result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        return false;

    // ��� ������ �����Ϳ� ���� �����͸� �����´�.
    dataPtr = (MatrixBufferType*)mappedResource.pData;

    // ��� ���ۿ� ����� ����
    dataPtr->worldMatrix      = worldMatrix;
    dataPtr->viewMatrix       = viewMatrix;
    dataPtr->projectionMatrix = projectionMatrix;

    // ��� ������ �������
    deviceContext->Unmap(_matrixBuffer, 0);

    // ���� ���̴������� ��� ���� ��ġ����
    bufferNumber = 0;

    // ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲ۴�.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

    return true;
}
/*****************************************************************************************************
render()�Լ����� setShaderParameter()���� ���� ȣ��Ǿ� ���̴��� ���ڵ��� �ùٷ� �����ϰ� �ȴ�.

�Է� ���̾ƿ��� �Է� ������� �����Ͽ�, GPU���� ������ �ڷᱸ���� �˰� �ȴ�.
���� �������۸� �׸��� ���� ����&�ȼ� ���̴��� �����Ѵ�.
���̴��� �����Ǹ� D3D����̽� ���ؽ�Ʈ���� DirectX11�� DrawIndexed() �Լ��� �����
�ﰢ���� �׷����� �ȴ�.
�� �Լ��� ȣ��� ���Ŀ�, ���� Ʃ�丮�� ���� �ʷϻ� �ﰢ���� �׷����� �ȴ�.
*****************************************************************************************************/
void ColorShader::renderShader(
    ID3D11DeviceContext* deviceContext,
    int                 indexCount)
{
    // ���� �Է� ���̾ƿ��� ����
    deviceContext->IASetInputLayout(_layout);

    // �ﰢ���� �׸� ����&�ȼ� ���̴��� ����.
    deviceContext->VSSetShader(_vertexShader, NULL, 0);
    deviceContext->PSSetShader(_pixelShader, NULL, 0);

    // ����
    deviceContext->DrawIndexed(indexCount, 0, 0);

    return;
}
