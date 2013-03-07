#include "World.h"


World::World(ID3D11Device *device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView, D3DXVECTOR2 Resolution)
{
	gState				=	(WorldState)Paused;
	gDeviceContext		=	deviceContext;
	gResolution.x		=	(int)Resolution.x;
	gResolution.y		=	(int)Resolution.y;
	
	gInput				=	new InputManager();

	
	MapManager*	MM		=	new MapManager();
	gGOManager			=	new GameObjectManager(MM);



	//	Create the camera, with all its settings
	gCamera			=	new Camera(	90 * ((float)D3DX_PI/180), 
									gResolution.x / gResolution.y, 
									0.5f, 
									500.0f
									);
	gCamera->SetInputManager(gInput);


	//Create the GraphicsManager
	gGraphicsManager	=	new GraphicsManager(device, deviceContext, renderTargetView, Resolution);
	gGraphicsManager->SetCamera(gCamera);
	gGraphicsManager->SetModelPath("Models");
	gGraphicsManager->SetTexturePath("Models\\Textures");
	//GameObjectManager->GetGameObjects() ist�llet f�r new vector<GameObject*>().
	gGraphicsManager->SetGameObjects(gGOManager->GetGameObjects());

	gGraphicsManager->LoadModels();
}


void World::Update(float deltaTime)
{
	//	Let the input update first so we know
	//	what has been pressed this update.
	gInput->Update();

	//	Update all the Game Objects
	gGOManager->Update(deltaTime);
	




	//	Update the camera last so it has the
	//	updated scene ready to adjust to.
	gCamera->Update(deltaTime);
	//SetCursorPos(gResolution.x * 0.5f, gResolution.y * 0.5f);
}

void World::Render()
{
	gGraphicsManager->Render();
}