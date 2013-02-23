#ifndef WORLD_H
#define WORLD_H	

#include "stdafx.h"
#include "InputManager.h"
#include "Camera.h"

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

	InputManager*	gInput;
	Camera*			gCamera;

	D3DXVECTOR2		gResolution;
public:
	World(ID3D11DeviceContext* deviceContext, D3DXVECTOR2 Resolution);


	void Update(float deltaTime);
	void Render(void);
};

#endif