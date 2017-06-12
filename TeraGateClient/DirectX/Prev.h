#pragma once
#ifdef _DEBUG
    #pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#endif
#define WIND32_LEAN_AND_MEAN
#pragma comment(lib, "winmm.lib")// playsound() 사용을 위해 선언
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
//#pragma comment(lib, "D3Dcompiler.lib")
// #pragma comment(lib, "D3Dcompiler.lib")
#include<time.h>        // 타이머 사용을 위해 선언
#include<windows.h>
#include<stdio.h>       // sprintf() 사용을 위해 선언
#include<stdlib.h>
#include<math.h>
#include<iostream>      // #include<fstream>, using std::ofstream 사용을 위해 선언
#include<fstream>       // unsing std::ofstream 사용을 위해 선언
#include<D3D11.h>       // #include<D3DX11.h> d3d11, d3dx 차이점이 뭐지?;
#include<D3DX11.h>      // D3DX11CompileFromFile()함수 사용가능. D3D11은 안됨.
#include<D3Dcompiler.h> // 쉐이더 컴파일 함수 지원
#include<D3DX10math.h>  // Direct3D Math 함수 지원
#include<D3DX11async.h> // CreateVertexShaderFromFile() 함수 헤더인데 왜 아예 안되지..
#include<d3dcommon.h>
#include<DXGI.h>
// #include<DirectXMath.h> // XMFLOAT3 구조체 사용가능..하긴 한데 그냥 D3DX10math.h의 D3DXVECTOR3 구조체 쓰자.. 연산자 오버로딩도 이것저것 되있다.
#define CLIENT_WIDTH  1280
#define CLIENT_HEIGHT 720
#define DEGREE_TO_RADIAN(degree) (degree * 0.0174532925f)
#define RADIAN_TO_DEGREE(radian) (radian * 57.295779513f)
#define SRAND() srand((unsigned)time(NULL))                     //time.h
#define SRAND_SEED(seed) srand((unsigned)time(NULL)*seed)       //time.h
#define RANDOM_S(STANDARD, SIZE) (STANDARD-(SIZE/2)+rand()%SIZE)//time.h
#define RANDOM_M(MIN, MAX) (MIN + rand()%(MAX+1-MIN))           //time.h
int     RANDOM_E(int exceptSize, int size);
const bool  FULL_SCREEN   = false;
const bool  VSYNC_ENABLED = true;
const float SCREEN_DEPTH  = 12000.f; // 이 이상 넘어가게 되면 알아서 컬링컬링 ><
const float SCREEN_NEAR   = 0.1f;
void infoD3DXMATRIX(D3DXMATRIX mtx);
void infoD3DXMATRIX(D3DXMATRIX mtx, char* desc);
void infoD3DXVECTOR3(D3DXVECTOR3 v);
void infoD3DXVECTOR3(D3DXVECTOR3 v, char* desc);
void ifFail(HRESULT hResult);