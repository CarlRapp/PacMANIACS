#pragma once
#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"
#include "ObjLoader.h"

class GraphicsManager
{
	ID3D11DeviceContext *gDeviceContext;
	ID3D11Device		*gDevice;

	Buffer				*gVertexBuffer;

public:
	GraphicsManager(void);
	~GraphicsManager(void);

	void LoadModels();

};
#endif
