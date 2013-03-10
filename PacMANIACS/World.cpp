#include "World.h"


World::World(ID3D11Device *device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView, HWND* hwnd, D3DXVECTOR2 Resolution)
{
	gState				=	(WorldState)Paused;
	gDeviceContext		=	deviceContext;
	gResolution.x		=	(int)Resolution.x;
	gResolution.y		=	(int)Resolution.y;
	
	gInput				=	new InputManager(hwnd);

	gCamera			=	new Camera(	90 * ((float)D3DX_PI/180), 
								gResolution.x / gResolution.y, 
								0.5f, 
								500.0f
								);

	gCamera->SetInputManager(gInput);

	gSoundManager = new SoundManager(gCamera, *hwnd);

	gSoundManager->SetSoundPath("Sounds");
	
	MapManager*	MM		=	new MapManager();
	gGOManager			=	new GameObjectManager(MM, gSoundManager);

	for(int i = 0; i < gGOManager->GetGameObjects()->size(); i++)
		if(gGOManager->GetGameObjects()->at(i)->GetName() == "Pacman")
			gPlayerObject	=	(Pacman*)gGOManager->GetGameObjects()->at(i);



	//	Create the camera, with all its settings

	gCamera->SetTarget(gPlayerObject);


	//Create the GraphicsManager
	gGraphicsManager	=	new GraphicsManager(device, deviceContext, renderTargetView, Resolution);
	gGraphicsManager->SetCamera(gCamera);
	gGraphicsManager->SetModelPath("Models");
	gGraphicsManager->SetTexturePath("Models\\Textures");
	//GameObjectManager->GetGameObjects() istället för new vector<GameObject*>().
	gGraphicsManager->SetGameObjects(gGOManager->GetGameObjects());

	gGraphicsManager->LoadModels();
	gGraphicsManager->LoadTexture("Flee_Texture.png");


	string key = gSoundManager->Loop("LoginScreenIntro4");
	gSoundManager->SetVolume(key, 0.7f);

	//gSoundManager->LoadSoundFile("LoginScreenIntro4");
	//gSoundManager->LoadSoundFile("Cherry");


	//string key1 = gSoundManager->LoopSound("Cherry", D3DXVECTOR3(-10, 0, 0));
	//string key2 = gSoundManager->PlaySound("LoginScreenIntro4", D3DXVECTOR3(10, 0, 0));

	//gSoundManager->StopSound(key1);
}

World::~World()
{
	gGOManager->~GameObjectManager();


}

void World::Update(float deltaTime)
{
	//	Let the input update first so we know
	//	what has been pressed this update.
	gInput->Update();

	if(gInput->IsKeyPressed('H'))
		gCamera->SetTarget(NULL);
	if(gInput->IsKeyPressed('G'))
		gCamera->SetTarget(gPlayerObject);

	if (!gGOManager->AllCandyGone() && gCamera->GetTarget() == gPlayerObject)
	{
		//Move gPlayerObject
		if (gInput->IsKeyPressed('W'))
			gPlayerObject->gNextMove = Pacman::NextMove::Forward;

		else if (gInput->IsKeyPressed('A'))
			gPlayerObject->gNextMove = Pacman::NextMove::Left;

		else if (gInput->IsKeyPressed('D'))
			gPlayerObject->gNextMove = Pacman::NextMove::Right;

		else if (gInput->IsKeyPressed('S'))
		{
			gPlayerObject->gNextMove	= Pacman::NextMove::Back;
			D3DXVECTOR2 playerTilePos	= gGOManager->GetTilePosition(gPlayerObject->GetPosition());
			D3DXVECTOR3 tileWorldPos	= gGOManager->GetWorldPosition(playerTilePos.x, 0, playerTilePos.y);
			vector<D3DXVECTOR3> availableMoves = gGOManager->GetAvailableMoves(playerTilePos.x, playerTilePos.y);
			availableMoves.push_back(tileWorldPos);
			gPlayerObject->CalculateMove(availableMoves);
		}

		if (gPlayerObject->AtDestination())
		{
			D3DXVECTOR2 playerTilePos = gGOManager->GetTilePosition(gPlayerObject->GetPosition());
			vector<D3DXVECTOR3> availableMoves = gGOManager->GetAvailableMoves(playerTilePos.x, playerTilePos.y);
			gPlayerObject->CalculateMove(availableMoves);			
		}
	}

	//	Update all the Game Objects
	gGOManager->Update(deltaTime);

	//	Update the camera last so it has the
	//	updated scene ready to adjust to.
	gCamera->Update(deltaTime);

	gSoundManager->Update();
	//SetCursorPos(gResolution.x * 0.5f, gResolution.y * 0.5f);


	//	If the game is over(won)
	if(gGOManager->AllCandyGone() && gPlayerObject != NULL)
	{
		if(gPlayerObject->GetState()->GetName() != "Idle")
		{
			gPlayerObject->SetObjectState(new IdleGameObjectState());

			D3DXVECTOR3	MapInfo		=	gGOManager->GetMapInfo();
			D3DXVECTOR3	Destination;

			//X * mapScale, Y, Z * -mapScale

			Destination.x	=	MapInfo.x*MapInfo.z*0.5f - MapInfo.z*0.5f;
			Destination.z	=	-MapInfo.x*MapInfo.z*0.5f + MapInfo.z*0.5f;
			Destination.y	=	10.0f;

			gPlayerObject->MoveTo(Destination);
			gCamera->SetPosition(Destination - D3DXVECTOR3(0, 0, 5));
			gCamera->SetForward(Destination - gCamera->GetPosition());
			gCamera->ChangeState(CameraState::Idle);
		}

		gPlayerObject->AddRotation(0, 4*deltaTime, 0);
	}
}

void World::Render()
{
	ostringstream ss;
	ss << gPlayerObject->GetPoints();
	string s(ss.str());


	if(!gPlayerObject->IsAlive())
	{
		string str = "Game over!";
		gGraphicsManager->DrawString(str, D3DXVECTOR2(gResolution.x * 0.5f - 50*str.length()*0.25f, gResolution.y * 0.3f), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 50);
	}
	else
	{
		gGraphicsManager->DrawString("Score:", D3DXVECTOR2(10, 10), D3DXVECTOR3(1,0,0), 20);
		gGraphicsManager->DrawString(s, D3DXVECTOR2(75,10), D3DXVECTOR3(1,0,0), 20);
	}
	gGraphicsManager->Render();
}