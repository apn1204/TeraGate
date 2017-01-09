#include"Model.h"
Model::Model()
{
    _vertexBuffer = 0;
    _indexBuffer  = 0;
}
Model::Model(const Model& other)
{

}
Model::~Model()
{

}

// ���� ���ۿ� �ε��� ������ �ʱ�ȭ �Լ��� ȣ��
bool Model::init(ID3D11Device* device)
{
    bool result;
    result = initBuffer(device);
    if (!result)
        return false;
    return true;
}

void Model::shut()
{
    shutBuffer();
    return;
}
// renderBuffer() �Լ��� ȣ���� ���� ���ۿ� �ε��� ���۸� �׷��� ���������ο� �־� �÷� ���̴��� �̵��� �׸� �� �ְ� �Ѵ�.
void Model::render(ID3D11DeviceContext* deviceContext)
{
    // ���� ���ۿ� �ε��� ���۸� �׷��Ƚ� ���������ο� �־� ȭ�鿡 �׸� �غ�
    renderBuffer(deviceContext);
    return;
}
// �ش� ���� �ε��� ���� ��ȯ
// �÷� ���̴����� ���� �׸��� ���� �ʿ�
int Model::getIndexCount()
{
    return _indexCount;
}
/*****************************************************************************************************
���� ���ۿ� �ε��� ���۸� �����ϴ� �۾� ����
������ ������ ���Ϸκ��� �� ������ �о�ͼ� ���۵��� ����� ���� �Ѵ�.
�ϴ� Ʃ�丮���̶� �ﰢ�� �ϳ��� �ٷ�� ������ ���� ����, �ε��� ���ۿ� ���� �����ϴ� �ϸ� ������.
*****************************************************************************************************/
bool Model::initBuffer(ID3D11Device* device)
{
    /*
    �̱��� ������Ʈ �Ŵ���
    ������Ʈ �� ���ؽ� 3�� ���ٰ� ����.
    3 * ������Ʈ ���� N ���� ���ؽ� �ʿ�.
    3N ���ؽ� �ʿ�.

    ���ؽ� ������ 3N���� �ʱ�ȭ�Ѵ�.
    ������Ʈ ��ġ�� ���� ���ؽ� ��ǥ�� �ʱ�ȭ�Ѵ�.
    */

    VertexType*            vertex;
    unsigned long*         index;
    D3D11_BUFFER_DESC      vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT                result;


    /*****************************************************************************************************
    ����, �ε��� �����͸� ��Ƶ� �� ���� �ӽ� �迭�� ����� ���߿� ���� ���۸� ������ �� ����Ѵ�.
    *****************************************************************************************************/
    // ���� �迭 ���� ����
    _vertexCount = 6;

    // �ε��� �迭 ���� ����
    _indexCount  = 6;

    // ���� �迭�� ����
    vertex = new VertexType[_vertexCount];
    if (!vertex)
        return false;

    // �ε��� �迭�� ����
    index = new unsigned long[_indexCount];
    if (!index)
        return false;

    /*****************************************************************************************************
    ����, �ε��� �迭�� �ﰢ���� �� ���� �� ������ ä�� �ִ´�.
    ������ ����, �̰��� �׸��� ���� ������ �ð�������� ������ �Ѵ�.
    ���� �ݽð� �������� ����ԵǸ� DirectX���� �� �ﰢ���� �ݴ����� �ٶ󺻴ٰ� �Ǵ��Ͽ�
    backface culling�� ���� �׷����� �ʰ� �ȴ�.
    ���� GPU���� ������ �׸����� �� �� �� ������ ����ϴ� ���� �߿��ϴ�.
    ���⼭�� ������ description�� �ۼ��ϱ� ������ ���� �����ְ� �ȴ�.
    *****************************************************************************************************/
    // �ﰢ��1
    // ���� �Ʒ�
    vertex[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  
    vertex[0].color    = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    // ��� ���
    vertex[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  
    vertex[1].color    = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    // ������ �Ʒ�
    vertex[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
    vertex[2].color    = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

    // �ﰢ��2
    // ���� �Ʒ�
    vertex[3].position = D3DXVECTOR3(-3.0f, -3.0f, -2.0f);
    vertex[3].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    // ��� ���
    vertex[4].position = D3DXVECTOR3(-2.0f, -1.0f, -2.0f);
    vertex[4].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    // ������ �Ʒ�
    vertex[5].position = D3DXVECTOR3(-1.0f, -3.0f, -2.0f);
    vertex[5].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

    // �ε��� �迭�� ���� �ִ´�.
    // ������� ���� �Ʒ�, ��� ���, ������ �Ʒ�
    index[0] = 0;
    index[1] = 1;
    index[2] = 2;

    index[3] = 3;
    index[4] = 4;
    index[5] = 5;

    /*****************************************************************************************************
    ����&�ε��� �迭�� ä�������Ƿ� �̸� �̿��� ����&�ε��� ���� ����
    �켱 ���ۿ� ���� description�� �ۼ��Ѵ�.
    description���� ����ũ�� ByteWidth�� ����Ÿ�� BindFlags�� ��Ȯ�� �Է��ؾ� �Ѵ�.
    �̸� ä������ �Ŀ��� ����&�ε��� �迭�� subresource �����Ϳ� �����Ѵ�.
    �� description�� subresource������, D3D����̽��� CreateBuffer �Լ��� ����� �� ������ �����͸� �޾ƿ´�.
    *****************************************************************************************************/
    // �������� description �ۼ�
    vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth           = sizeof(VertexType) * _vertexCount;
    vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags      = 0;
    vertexBufferDesc.MiscFlags           = 0;
    vertexBufferDesc.StructureByteStride = 0;
    
    // ���� �����͸� ����Ű�� ���� ���ҽ� ����ü �ۼ�
    vertexData.pSysMem          = vertex;
    vertexData.SysMemPitch      = 0;
    vertexData.SysMemSlicePitch = 0;

    // ���� ���۸� ����
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
    if (FAILED(result))
        return false;

    // �ε��� ������ description �ۼ�
    indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth           = sizeof(unsigned long) * _indexCount;
    indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags      = 0;
    indexBufferDesc.MiscFlags           = 0;
    indexBufferDesc.StructureByteStride = 0;

    // �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü �ۼ�
    indexData.pSysMem          = index;
    indexData.SysMemPitch      = 0;
    indexData.SysMemSlicePitch = 0;

    // �ε��� ���� ����
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
    if (FAILED(result))
        return false;

    /*****************************************************************************************************
    ����&�ε��� ���� ���� �Ŀ��� �̹� ���� ����Ǿ� �ʿ䰡 ������
    ����&�ε��� �迭�� �������ش�.
    *****************************************************************************************************/
    delete[] vertex;
    vertex = 0;

    delete[] index;
    index = 0;

    return true;
}
// initBuffer()���� ������� ����&�ε��� ���۸� �����Ѵ�
void Model::shutBuffer()
{
    if (_indexBuffer)
    {
        _indexBuffer->Release();
        _indexBuffer = 0;
    }
    if (_vertexBuffer)
    {
        _vertexBuffer->Release();
        _vertexBuffer = 0;
    }
    return;
}
/*****************************************************************************************************
renderBuffer()�� render()�Լ����� ȣ��ȴ�.
����&�ε��� ���۸� GPU ������� ���۷μ� Ȱ��ȭ��Ű�� ���̴�.
�ϴ� GPU�� Ȱ��ȭ�� �������۸� ������ �Ǹ� ���̴��� �̿��� ������ ������ �׸� �� �ְ� �ȴ�.
���� �� �Լ������� ������ �ﰢ��, ����, ��ä�� �� � ������� �׸��� �����Ѵ�.
*****************************************************************************************************/
void Model::renderBuffer(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;// ��������, �����̶�� ������ �ǹ̸� ������ �ִ�.
    unsigned int offset;

    // ���������� ������ �������� �����Ѵ�.
    stride = sizeof(VertexType);
    offset = 0;

    // input assembler�� ���� ���۸� Ȱ��ȭ�� �׷��� �� �ְ� �Ѵ�.
    deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

    // input assembler�� �ε��� ���۸� Ȱ��ȭ�� �׷��� �� �ְ� �Ѵ�.
    deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // ���� ���۷� �׸� �⺻���� �ﰢ������ �����Ѵ�.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}