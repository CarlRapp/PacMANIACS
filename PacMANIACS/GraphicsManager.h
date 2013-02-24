#pragma once
#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "GameObject.h"

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

	ID3D11DeviceContext *gDeviceContext;
	ID3D11Device		*gDevice;

	Buffer				*gVertexBuffer;

	map<string, IndexInfo> gIndexMap;
	string modelPath;

public:
	GraphicsManager(void);
	~GraphicsManager(void);

	void LoadModels(vector<GameObject> *gameObjects);
	void Render();

};
#endif
