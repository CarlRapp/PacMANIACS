#include "World.h"


World::World(ID3D11Device *device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView, HWND hwnd, D3DXVECTOR2 Resolution)
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
	//GameObjectManager->GetGameObjects() istället för new vector<GameObject*>().
	gGraphicsManager->SetGameObjects(gGOManager->GetGameObjects());

	gGraphicsManager->LoadModels();

	gSoundManager = new SoundManager(gCamera, hwnd);
	gSoundManager->SetSoundPath("Sounds");

	gSoundManager->LoadSoundFile("LoginScreenIntro4");
	gSoundManager->LoadSoundFile("Cherry");

	D3DXVECTOR3	DEST	=	gGOManager->GetWorldPosition(1, 0, 1);
	gGOManager->GetGameObjects()->at(14)->SetDestination(DEST.x, 0, DEST.z);

	string key1 = gSoundManager->LoopSound("Cherry", D3DXVECTOR3(-10, 0, 0));
	//string key2 = gSoundManager->PlaySound("LoginScreenIntro4", D3DXVECTOR3(10, 0, 0));

	//gSoundManager->StopSound(key1);

	for(int i = 0; i < gGOManager->GetGameObjects()->size(); i++)
		if(gGOManager->GetGameObjects()->at(i)->GetName() == "Pacman")
			gCamera->SetTarget(gGOManager->GetGameObjects()->at(i));
}


void World::Update(float deltaTime)
{
	//	Let the input update first so we know
	//	what has been pressed this update.
	gInput->Update();

	GameObject* GO = NULL;


	for(int i = 0; i < gGOManager->GetGameObjects()->size(); i++)
		if(gGOManager->GetGameObjects()->at(i)->GetName() == "Pacman")
			GO = gGOManager->GetGameObjects()->at(i);

	if(gInput->IsKeyDown('W'))
	{
		D3DXVECTOR3	S;
		D3DXVec3TransformCoord(&S, &D3DXVECTOR3(0,0,-1), &GO->GetRotationMatrix());
		
		
		if(gInput->IsKeyDown(VK_SHIFT))
			S = 9*S;
		else
			S = 3*S;

		GO->Move(S.x * deltaTime, 0, S.z * deltaTime);
	}

	if(gInput->IsKeyDown('S'))
	{
		D3DXVECTOR3	S;
		D3DXVec3TransformCoord(&S, &D3DXVECTOR3(0,0,-1), &GO->GetRotationMatrix());
		
		
		if(gInput->IsKeyDown(VK_SHIFT))
			S = 9*S;
		else
			S = 3*S;

		GO->Move(-S.x * deltaTime, 0, -S.z * deltaTime);
	}

	if(gInput->IsKeyDown('A'))
		GO->SetRotation(0, -deltaTime * 1.5f, 0);
	else if(gInput->IsKeyDown('D'))
		GO->SetRotation(0, deltaTime * 1.5f, 0);

	if(gInput->IsKeyPressed('H'))
		gCamera->SetTarget(NULL);
	if(gInput->IsKeyPressed('G'))
		gCamera->SetTarget(GO);

	//	Update all the Game Objects
	gGOManager->Update(deltaTime);




	//	Update the camera last so it has the
	//	updated scene ready to adjust to.
	gCamera->Update(deltaTime);

	gSoundManager->Update();
	//SetCursorPos(gResolution.x * 0.5f, gResolution.y * 0.5f);
}

void World::Render()
{
	gGraphicsManager->Render();
}