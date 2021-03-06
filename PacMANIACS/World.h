#ifndef WORLD_H
#define WORLD_H	

#include "stdafx.h"
#include "InputManager.h"
#include "Camera.h"
#include "GraphicsManager.h"
#include "MapManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"

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
	GameObjectManager*	gGOManager;
	Camera*				gCamera;
	SoundManager*		gSoundManager;


	D3DXVECTOR2		gResolution;

	Pacman*		gPlayerObject;
public:
	World(ID3D11Device *device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView, HWND* hwnd, D3DXVECTOR2 Resolution);
	~World(void);

	void Update(float deltaTime);
	void Render(void);
};

#endif