#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(MapManager* MapData)
{
	gMap	=	MapData->GetMap();


	StartConvert(MapData);
}

void GameObjectManager::StartConvert(MapManager* MapData)
{
	allGameObjects		=	new vector<GameObject*>();
	moveableObjects		=	new vector<GameObject*>();
	stationaryObjects	=	new vector<GameObject*>();

	int		mWidth	=	MapData->GetWidth();
	int		mHeight	=	MapData->GetHeight();

	for(int X = 0; X < mWidth; ++X)
	{
		for(int Y = 0; Y < mHeight; ++Y)
		{
			char	currentChar	=	gMap[X][Y];

			if(MapData->GetGameObjectList().count(currentChar) > 0)
			{
				string	ObjectName	=	MapData->GetGameObjectList().at(currentChar);
				GameObject*	GO	=	NULL;
				GO	=	ConvertStringToGameObject(ObjectName);
				GO->SetScale(0.5f, 0.5f, 0.5f);
				GO->MoveTo(Y, 1, X);
				GO->SetRotation((X+1)*(Y-1), 0, 0);
				
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


					GameObject*	GO2	=	new Candy();
					GO2->SetScale(0.5f, 0.5f, 0.5f);
					GO2->MoveTo(Y, 1, X);

					//	Add candy to both lists
				//	stationaryObjects->push_back(GO2);
				//	allGameObjects->push_back(GO2);
				}
			}
			else if (currentChar == '0')
			{
				GameObject*	GO2	=	new Candy();
				GO2->SetScale(0.5f, 0.5f, 0.5f);
				GO2->MoveTo(Y, 1, X);

				//	Add candy to both lists
				stationaryObjects->push_back(GO2);
				allGameObjects->push_back(GO2);
			}

			if(currentChar == '1')
			{
				GameObject*	WALL	=	new Wall();
				WALL->MoveTo(Y, 0, X);

				allGameObjects->push_back(WALL); 
			}
			else
			{
				GameObject* FLOOR = new Floor();
				FLOOR->MoveTo(Y, 0, X);

				allGameObjects->push_back(FLOOR);
			}
		}
	}
}

GameObject* GameObjectManager::ConvertStringToGameObject(string GOName)
{
	GameObject*	GO	=	NULL;

	if(GOName == "Pacman")
		GO	=	new GameObject();
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

		if(A->GetName() == "Pacman")
			tPacman	=	A;

		for each (GameObject *B in *moveableObjects)
			if(A != B)
				if(A->IsColliding(B))
					HandleCollision(A, B);
	}

	
	for each (GameObject *A in *stationaryObjects)
	{
		A->Update(deltaTime);

		if(tPacman != NULL)
			if(A->IsColliding(tPacman))
				HandleCollision(A, tPacman);
	}
}

void GameObjectManager::HandleCollision(GameObject* GoA, GameObject* GoB)
{

}

vector<GameObject*>* GameObjectManager::GetGameObjects()
{
	return allGameObjects;
}