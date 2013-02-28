#pragma once
#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "GameObject.h"
#include "Camera.h"

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

	ID3D11DeviceContext		*gDeviceContext;
	ID3D11Device			*gDevice;

	Buffer					*gVertexBuffer;

	map<string, IndexInfo>	gIndexMap;
	string modelPath;

	Camera					*gCamera;
	vector<GameObject>		*gGameObjects;

public:
	GraphicsManager(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	~GraphicsManager(void);

	void SetCamera(Camera *camera);
	void SetGameObjects(vector<GameObject> *gameObjects);

	void LoadModels();
	void Render();

};
#endif
