#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"
#include "ObjLoader.h"
#include "Camera.h"
#include "GameObject.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>


#define WINDOW_WIDTH 1280
#define WINDOW_Height 720
using namespace std;

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HWND					g_hWndMain				= NULL;

IDXGISwapChain*         g_SwapChain				= NULL;
ID3D11RenderTargetView* g_RenderTargetView		= NULL;
ID3D11Texture2D*        g_DepthStencil			= NULL;
ID3D11DepthStencilView* g_DepthStencilView		= NULL;
ID3D11Device*			g_Device				= NULL;
ID3D11DeviceContext*	g_DeviceContext			= NULL;

Camera*					camera					= new Camera(D3DXVECTOR3(0, 0, -150), 
															D3DXVECTOR3(0, 0, 1), 5);
<<<<<<< HEAD
/*Model*					model					= new Model();
Model*					plane					= new Model();

Model*					lampSphere				= new Model();*/
D3DXVECTOR3				lampPosition			= D3DXVECTOR3(0, 0, -50);


char*					g_Title					= "3D Labb 2, OBJ Reader";
=======

char*					g_Title					= "Pacman::Reloaded";
>>>>>>> 5c1de7b2165a946e219c76ddfed4c31053735a72
ObjLoader*				objLoader				= NULL;
GameObject*				gameObject				=	new GameObject();

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
bool				InitWndApp(HINSTANCE hInstanceHandle, int show);
LRESULT CALLBACK	WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int					Run();
HRESULT				Render(float deltaTime);
HRESULT				Update(float deltaTime);
HRESULT				InitDirect3D();
char*				FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel);

//Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;

	if (!InitWndApp(hInstance, nShowCmd))
	{
		::MessageBox(0, "Initalaization Failed", "Error", MB_OK);
		return 0;
	}
	if( FAILED( InitDirect3D() ) )
	{
		::MessageBox(0, "Failed to initalize Direct3D", "Error", MB_OK);
		return 0;
	}
	LPRECT Rect = new RECT();
	GetWindowRect(g_hWndMain, Rect);
	int centerX = (Rect->left + Rect->right) * 0.5f;
	int centerY = (Rect->top + Rect->bottom) * 0.5f;
	SetCursorPos(centerX, centerY);
	ShowCursor(false);	

	camera->Update(0, D3DXVECTOR2(0, 0));

	return Run();
}

bool InitWndApp(HINSTANCE hInstanceHandle, int show)
{
	WNDCLASS wc;
	wc.style				= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc			= WndProc;
	wc.cbClsExtra			= 0;
	wc.cbWndExtra			= 0;
	wc.hInstance			= hInstanceHandle;
	wc.hIcon				= ::LoadIconA(0, IDI_APPLICATION);
	wc.hCursor				= ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground		= static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName			= 0;
	wc.lpszClassName		= "Window";

	if (!RegisterClass(&wc))
	{
		::MessageBox(0, "Failed to register WNDCLASS", 0, MB_OK);
		return 0;
	}
	RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_Height };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	g_hWndMain = ::CreateWindow("Window", 
							g_Title, 
							WS_OVERLAPPEDWINDOW, 
							CW_USEDEFAULT, 
							CW_USEDEFAULT, 
							rc.right - rc.left,
							rc.bottom - rc.top,
							0, 
							0, 
							hInstanceHandle, 
							0);

	if (g_hWndMain == 0)
	{
		::MessageBox(0, "Failed to create WNDCLASS", 0, MB_OK);
		return 0;
	}

	::ShowWindow(g_hWndMain, show);
	::UpdateWindow(g_hWndMain);

	return true;
}

HRESULT InitDirect3D()
{
	HRESULT hr = S_OK;;

	int screenWidth = WINDOW_WIDTH;
	int screenHeight = WINDOW_Height;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverType;

	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]); //same as ARRAYSIZE(x) macro

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = screenWidth;
	sd.BufferDesc.Height = screenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWndMain;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_FEATURE_LEVEL initiatedFeatureLevel;
	
	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevelsToTry,
			ARRAYSIZE(featureLevelsToTry),
			D3D11_SDK_VERSION,
			&sd,
			&g_SwapChain,
			&g_Device,
			&initiatedFeatureLevel,
			&g_DeviceContext);

		if( SUCCEEDED( hr ) )
			break;
	}
	
	if( FAILED(hr) )
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer;
	hr = g_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer );
	if( FAILED(hr) )
		return hr;

	hr = g_Device->CreateRenderTargetView( pBackBuffer, NULL, &g_RenderTargetView );
	pBackBuffer->Release();
	if( FAILED(hr) )
		return hr;


	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = screenWidth;
	descDepth.Height = screenHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_Device->CreateTexture2D( &descDepth, NULL, &g_DepthStencil );
	if( FAILED(hr) )
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_Device->CreateDepthStencilView( g_DepthStencil, &descDSV, &g_DepthStencilView );
	if( FAILED(hr) )
		return hr;


	g_DeviceContext->OMSetRenderTargets( 1, &g_RenderTargetView, g_DepthStencilView );

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)screenWidth;
	vp.Height = (float)screenHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_DeviceContext->RSSetViewports( 1, &vp );

	return S_OK;
}

bool firstUpdate;
int Run()
{
	firstUpdate = true;
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;

	while (WM_QUIT != msg.message)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;

			if (firstUpdate)
				firstUpdate = false;
			else
			{
				//render
				Update(dt);
				Render(dt);
			}
			prevTimeStamp = currTimeStamp;
		}
	}

	return (int)msg.wParam;
}


void CloseApplication(HWND hWnd)
{

	::DestroyWindow(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_ESCAPE:
					CloseApplication(hWnd);
			}
			return 0;
	}
	
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
float angle = 0.0f;
HRESULT Update(float deltaTime)
{
	float speed = deltaTime / 3;
	D3DXMATRIX rot;
	D3DXMatrixRotationZ(&rot, speed);

	speed *= 3;

	D3DXMatrixRotationY(&rot, speed);

	LPRECT Rect = new RECT();
	GetWindowRect(g_hWndMain, Rect);
	LPPOINT mousePos = new POINT();
	GetCursorPos(mousePos);


	int centerX = (Rect->left + Rect->right) * 0.5f;
	int centerY = (Rect->top + Rect->bottom) * 0.5f;
	D3DXVECTOR2 mouseMovement = D3DXVECTOR2(mousePos->x - centerX, centerY - mousePos->y);

	SetCursorPos(centerX, centerY);
	camera->Update(deltaTime, mouseMovement);

	char title[255];
	sprintf_s(title, sizeof(title), "Pacman::Reloaded | FPS: %d",
		(int)(1.0f / deltaTime));

	SetWindowText(g_hWndMain, title);

	return S_OK;
}

HRESULT Render(float deltaTime)
{

	//	Clear the render target
	float ClearColor[4] = {0.05f,  0.05f, 0.05f, 1.0f};
	g_DeviceContext->ClearRenderTargetView( g_RenderTargetView, ClearColor );

	//	Clear the depth stencil
	g_DeviceContext->ClearDepthStencilView( g_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	LPRECT Rect = new RECT();
	GetWindowRect(g_hWndMain, Rect);

	float width = Rect->right - Rect->left;
	float height = Rect->bottom - Rect->top;

	if(FAILED(g_SwapChain->Present( 0, 0 )))
		return E_FAIL;

	return S_OK;
}


char* FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel)
{
	if(featureLevel == D3D_FEATURE_LEVEL_11_0)
		return "11.0";
	if(featureLevel == D3D_FEATURE_LEVEL_10_1)
		return "10.1";
	if(featureLevel == D3D_FEATURE_LEVEL_10_0)
		return "10.0";

	return "Unknown";
}
