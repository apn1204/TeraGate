#pragma once
#include<D3D11.h>
#include<D3DX10math.h>
class Model
{
public:
    Model();
    Model(const Model&);
    ~Model();
public:
    // render() �Լ��� �׷��� ī�忡 �𵨵��� ���� ������ �ְ� �÷� ���̴��� �׸� �غ� �Ѵ�.
    bool init(ID3D11Device*);
    void shut();
    void render(ID3D11DeviceContext*);
    int  getIndexCount();
private:
    // �Ʒ� �Լ����� 3D���� ����&�ε��� ���۵��� �ʱ�ȭ, ��������� �����Ѵ�.
    bool initBuffer(ID3D11Device*);
    void shutBuffer();
    void renderBuffer(ID3D11DeviceContext*);
private:
    ID3D11Buffer *_vertexBuffer, *_indexBuffer;
    int           _vertexCount,   _indexCount;
private:
    // Model Ŭ������ ���� ���ۿ� ����� ���� ����ü ����.
    // ����� Ÿ���� ���߿� ���� ColorShader Ŭ�������� ����� �Ͱ� �� ������ ���ƾ� �Ѵ�.
    struct VertexType
    {
        D3DXVECTOR3 position;
        D3DXVECTOR4 color;
    };
};