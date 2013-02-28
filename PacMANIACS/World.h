#ifndef WORLD_H
#define WORLD_H	

#include "stdafx.h"
#include "InputManager.h"
#include "Camera.h"
#include "GraphicsManager.h"

#include <string>

enum WorldState
{
	Active,
	Paused
};

class World
{
private:
	WorldState	gState;

	ID3D11DeviceContext*	gDeviceContext;

	InputManager*		gInput;
	GraphicsManager*	gGraphicsManager;
	Camera*				gCamera;

	D3DXVECTOR2		gResolution;
public:
	World(ID3D11Device *device, ID3D11DeviceContext* deviceContext, D3DXVECTOR2 Resolution);


	void Update(float deltaTime);
	void Render(void);
};

#endif