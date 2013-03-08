#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(MapManager* MapData, SoundManager* soundManager)
{
	mapScale		=	3.0f;
	gMap			=	MapData->GetMap();
	gSoundManager	=	soundManager;

	StartConvert(MapData);
}

void GameObjectManager::StartConvert(MapManager* MapData)
{
	allGameObjects		=	new vector<GameObject*>();
	moveableObjects		=	new vector<GameObject*>();
	stationaryObjects	=	new vector<GameObject*>();

	mapWidth	=	MapData->GetWidth();
	mapHeight	=	MapData->GetHeight();

	GameObject*	tPacman	=	NULL;

	for(int X = 0; X < mapWidth; ++X)
	{
		for(int Y = 0; Y < mapHeight; ++Y)
		{
			char	currentChar	=	gMap[X][Y];
			int	xPos	=	mapScale * X;
			int	yPos	=	-mapScale * Y;

			if(MapData->GetGameObjectList().count(currentChar) > 0)
			{
				string	ObjectName	=	MapData->GetGameObjectList().at(currentChar);
				GameObject*	GO	=	NULL;
				GO	=	ConvertStringToGameObject(ObjectName);
				GO->SetScale(1.0f, 1.0f, 1.0f);
				GO->MoveTo(xPos, 1, yPos);

				if(ObjectName == "Pacman")
					tPacman	=	GO;
				
				if(GO->IsStationary())
				{
					//	Add stationary to both lists
					stationaryObjects->push_back(GO);
					allGameObjects->push_back(GO);
				}
				else
				{
					//	Add moveable to both lists
					moveableObjects->push_back(GO);
					allGameObjects->push_back(GO);

					GameObject*	candy	=	new Candy();
					candy->SetScale(0.5f, 0.5f, 0.5f);
					candy->MoveTo(xPos, 1, yPos);

					//	Add candy to both lists
					stationaryObjects->push_back(candy);
					allGameObjects->push_back(candy);
				}
			}
			else if(currentChar != '1')
			{
				GameObject*	candy	=	new Candy();
				candy->SetScale(0.5f, 0.5f, 0.5f);
				candy->MoveTo(xPos, 1, yPos);

				//	Add candy to both lists
				stationaryObjects->push_back(candy);
				allGameObjects->push_back(candy);
			}

			if(currentChar == '1')
			{
				GameObject*	WALL	=	new Wall();
				WALL->MoveTo(xPos, 0, yPos);
				WALL->SetScale(mapScale, 1, mapScale);
				allGameObjects->push_back(WALL); 
			}
			else
			{
				GameObject* FLOOR = new Floor();
				FLOOR->MoveTo(xPos, 0, yPos);
				FLOOR->SetScale(mapScale, 3, mapScale);
				allGameObjects->push_back(FLOOR);
			}
		}
	}

	if(tPacman != NULL)
		for(int i = 0; i < moveableObjects->size(); i++)
			if(moveableObjects->at(i)->GetName() == "Ghost")
			{
				Ghost* GO		=	(Ghost*)moveableObjects->at(i);
				D3DXVECTOR2	POS	=	GetTilePosition(GO->GetPosition());
				GO->SetTarget(tPacman);
				GO->CalculateMove(GetAvailableMoves(POS.x, POS.y));
				if (gSoundManager != NULL)
					GO->soundKey = gSoundManager->LoopSound("Cherry", GO->GetPosition());
			}
}

GameObject* GameObjectManager::ConvertStringToGameObject(string GOName)
{
	GameObject*	GO	=	NULL;

	if(GOName == "Pacman")
		GO	=	new Pacman();
	else if(GOName == "Cherry")
		GO	=	new Cherry();
	else if(GOName == "Ghost1")
	{
		GO	=	new Ghost();
		((Ghost*)(GO))->SetAIState(new StupidGhostAIState());
	}
	else if(GOName == "Ghost2")
	{
		GO	=	new Ghost();
		((Ghost*)(GO))->SetAIState(new NormalGhostAIState());
	}
	else if(GOName == "Ghost3")
	{
		GO	=	new Ghost();
		((Ghost*)(GO))->SetAIState(new SmartGhostAIState());
	}
	else if(GOName == "Ghost4")
	{
		GO	=	new Ghost();
		((Ghost*)(GO))->SetAIState(new GeniusGhostAIState());
	}

	return GO;
}

void GameObjectManager::Update(float deltaTime)
{
	GameObject*	tPacman	=	NULL;

	for each (GameObject *A in *moveableObjects)
	{
		A->Update(deltaTime);

		if(A->AtDestination())
		{
			if(A->GetName() == "Ghost")
			{
				Ghost*	GO	=	((Ghost*)(A));
				D3DXVECTOR2	Pos	=	GetTilePosition(A->GetPosition());

				if(IsTileCrossing(Pos.x, Pos.y) || IsTileCorner(Pos.x, Pos.y))
				{
					GO->CalculateMove(GetAvailableMoves(Pos.x, Pos.y));
				}
				else
				{
					D3DXVECTOR3 Vel;
					D3DXVec3Normalize(&Vel, &GO->GetVelocity());

					Vel	*=	mapScale;
					D3DXVECTOR3	newD	=	GO->GetPosition() + Vel;

					GO->SetDestination(newD);
				}

			}
		}

		if(A->GetName() == "Pacman")
			tPacman	=	A; 

		else if (A->GetName() == "Ghost")
		{
			Ghost* ghost = (Ghost*)A;
			gSoundManager->SetSoundPosition(ghost->soundKey, ghost->GetPosition());
		}
		/*
		for each (GameObject *B in *moveableObjects)
			if(A != B)
				if(A->IsColliding(B))
					HandleCollision(A, B);
					*/
	}

	bool	cherryEaten	=	false;
	for(int i = stationaryObjects->size() - 1; i >= 0; --i)
	{
		GameObject*	A	=	stationaryObjects->at(i);

		A->Update(deltaTime);


		if(tPacman != NULL)
			if(A->IsColliding(tPacman))
			{
				for(int n = 0; n < allGameObjects->size(); ++n)
					if(allGameObjects->at(n) == A)
					{
						allGameObjects->erase(allGameObjects->begin() + n);
						break;
					}

				if(A->GetName() == "Cherry")
					AlertGhosts();

				stationaryObjects->erase(stationaryObjects->begin() + i);
			}
	}
}

void GameObjectManager::HandleCollision(GameObject* GoA, GameObject* GoB)
{
}

void GameObjectManager::AlertGhosts()
{
	for each (GameObject *A in *moveableObjects)
	{
		if(A->GetName() == "Ghost")
			((Ghost*)A)->FleeTarget();
	}
}

vector<GameObject*>* GameObjectManager::GetGameObjects()
{
	return allGameObjects;
}

D3DXVECTOR2 GameObjectManager::GetTilePosition(D3DXVECTOR3 Pos)
{
	return D3DXVECTOR2((int)((Pos.x + mapScale * 0.5f)/mapScale), (int)((Pos.z - mapScale * 0.5f)/-mapScale));
}

D3DXVECTOR3	GameObjectManager::GetWorldPosition(int X, int Y, int Z)
{
	return D3DXVECTOR3(X * mapScale, Y, Z * -mapScale);
}

bool GameObjectManager::IsFloor(int X, int Z)
{
	if(X >= 0 && X < mapWidth)
		if(Z >= 0 && Z < mapHeight)
			if(gMap[X][Z] != '1')
				return true;

	return false;
}

bool GameObjectManager::IsTileCrossing(int X, int Z)
{
	int	waysToGo	=	0;
	waysToGo		+=	IsFloor(X - 1, Z) ? 1 : 0;
	waysToGo		+=	IsFloor(X + 1, Z) ? 1 : 0;
	waysToGo		+=	IsFloor(X, Z - 1) ? 1 : 0;
	waysToGo		+=	IsFloor(X, Z + 1) ? 1 : 0;

	return (waysToGo > 2);
}

bool GameObjectManager::IsTileCorner(int X, int Z)
{
	vector<D3DXVECTOR3>	ways = vector<D3DXVECTOR3>();

	int	waysToGo	=	0;

	if(IsFloor(X - 1, Z))
		ways.push_back(D3DXVECTOR3(X - 1, 0, Z));
	if(IsFloor(X + 1, Z))
		ways.push_back(D3DXVECTOR3(X + 1, 0, Z));
	if(IsFloor(X, Z - 1))
		ways.push_back(D3DXVECTOR3(X, 0, Z - 1));
	if(IsFloor(X, Z + 1))
		ways.push_back(D3DXVECTOR3(X, 0, Z + 1));

	if(ways.size() == 1)
		return true;

	if(ways.size() == 2)
	{
		D3DXVECTOR3	v1	=	ways.at(0);
		D3DXVECTOR3	v2	=	ways.at(1);
		
		if(v1.x != v2.x && v1.z != v2.z)
			return true;
	}

	return false;
}

vector<D3DXVECTOR3> GameObjectManager::GetAvailableMoves(int X, int Z)
{
	vector<D3DXVECTOR3> moves = vector<D3DXVECTOR3>();

	if(IsFloor(X - 1, Z))
		moves.push_back(GetWorldPosition(X - 1, 0, Z));
	if(IsFloor(X + 1, Z))
		moves.push_back(GetWorldPosition(X + 1, 0, Z));
	if(IsFloor(X, Z - 1))
		moves.push_back(GetWorldPosition(X, 0, Z - 1));
	if(IsFloor(X, Z + 1))
		moves.push_back(GetWorldPosition(X, 0, Z + 1));

	return moves;
}