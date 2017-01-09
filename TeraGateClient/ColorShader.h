#pragma once
#include<D3D11.h>
#include<D3DX10math.h>
#include<D3DX11async.h>
#include<fstream>
using namespace std;
/*****************************************************************************************************
GPU�� �����ϴ� 3D�𵨵��� �׸��� �� ����ϴ� HLSL ���̴��� ȣ���ϴ� Ŭ����
*****************************************************************************************************/
class ColorShader
{
public:
    ColorShader();
    ColorShader(const ColorShader&);
    ~ColorShader();
public:
    // ���̴��� �ʱ�ȭ�� �������� ����
    bool init(ID3D11Device*, HWND);
    void shut();
    bool render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

private:
    bool initShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void shutShader();
    void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

    bool setShaderParameter(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
    void renderShader(ID3D11DeviceContext*, int);
private:
    ID3D11VertexShader* _vertexShader;
    ID3D11PixelShader*  _pixelShader;
    ID3D11InputLayout*  _layout;
    ID3D11Buffer*       _matrixBuffer;
private:
    // ���� ���̴��� ����cBuffer ����ü�� ����
    // ���̴��� �ùٸ� �������� �ϱ� ���� �� typedef���� ����ü�� �ݵ��
    // ���� ���̴��� ���� ����ü�� �����ؾ� �Ѵ�.
    struct MatrixBufferType
    {
        D3DXMATRIX worldMatrix;
        D3DXMATRIX viewMatrix;
        D3DXMATRIX projectionMatrix;
    };
};