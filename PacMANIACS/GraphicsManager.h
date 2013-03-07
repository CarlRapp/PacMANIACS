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

//for testing
#include "Ghost.h"

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
	typedef map<string, IndexInfo>					MAP_INDEX;
	typedef map<string, ID3D11ShaderResourceView*>	MAP_RESOURCE;

	MAP_INDEX					gIndexMap;
	MAP_RESOURCE				gTextureMap;


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

	ID3D11ShaderResourceView* LoadShaderResourceView(string path);

	void RenderLevel();

public:
	GraphicsManager(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ID3D11RenderTargetView* renderTargetView, D3DXVECTOR2 resolution);
	~GraphicsManager(void);

	void SetCamera(Camera *camera);
	void SetGameObjects(vector<GameObject*> *gameObjects);
	
	void SetModelPath(string path);
	void SetTexturePath(string path);

	void LoadModels();
	void Render();

	void CreateLevelBuffer(MapManager* Map);
};
#endif
