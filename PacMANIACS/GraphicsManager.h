#pragma once
#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "GameObject.h"
#include "Camera.h"
#include "MapManager.h"
#include "FW1FontWrapper.h"

//for testing
#include "Ghost.h"

#pragma comment (lib, "FW1FontWrapper.lib")

class GraphicsManager
{
	struct ModelInfo
	{
		vector<Vertex> verticies;
		string name;
	};

	struct IndexInfo
	{
		int start;
		int count;
	};

	typedef map<string, vector<Vertex>>				MAP_VERTEX;
	typedef map<string, IndexInfo*>					MAP_INDEX;
	typedef map<string, ID3D11ShaderResourceView*>	MAP_RESOURCE;

	MAP_INDEX					*gIndexMap;

	MAP_RESOURCE				*gTextureMap;


	ID3D11DeviceContext		*gDeviceContext;
	ID3D11Device			*gDevice;

	ID3D11RenderTargetView* gRenderTargetView;

	ID3D11DepthStencilView* gDepthStencilView;

	Buffer					*gVertexBuffer;
	Shader					*gShader;

	string modelPath;
	string texturePath;

	Camera					*gCamera;
	vector<GameObject*>		*gGameObjects;
	
	IFW1FontWrapper			*pFontWrapper;

	ID3D11ShaderResourceView* LoadShaderResourceView(string path);

	void RenderLevel();
	UINT RGBToColorCode(D3DXVECTOR3 color);

public:
	GraphicsManager(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ID3D11RenderTargetView* renderTargetView, D3DXVECTOR2 resolution);
	~GraphicsManager(void);

	void SetCamera(Camera *camera);
	void SetGameObjects(vector<GameObject*> *gameObjects);
	
	void SetModelPath(string path);
	void SetTexturePath(string path);

	void LoadModels();
	void LoadTexture(string Texture);

	void Render();

	void DrawString(string text, D3DXVECTOR2 position, D3DXVECTOR3 color, float size);

	void CreateLevelBuffer(MapManager* Map);
	
};
#endif
