#pragma once
#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"
#include "ObjLoader.h"

class GraphicsManager
{
	struct IndexInfo
	{
		int start;
		int count;
	};

	ID3D11DeviceContext *gDeviceContext;
	ID3D11Device		*gDevice;

	Buffer				*gVertexBuffer;

	map<string, IndexInfo> gIndexMap;

public:
	GraphicsManager(void);
	~GraphicsManager(void);

	void LoadModels();
	void Render();

};
#endif
