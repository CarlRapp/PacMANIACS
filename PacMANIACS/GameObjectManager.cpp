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
				GO->MoveTo(X, 2, Y);
				
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


					cout << "ADD CANDY A(UNDER THE CREATED GAMEOBJECT)" << endl;
					GameObject*	GO2	=	NULL;
					GO2	=	new GameObject(); //	<---- Change to Candy when implemented!!!!!!!!!
					GO2->MoveTo(X, 2, Y);

					//	Add candy to both lists
					stationaryObjects->push_back(GO2);
					allGameObjects->push_back(GO2);
				}
			}
			else if (currentChar == '0')
			{
				cout << "ADD CANDY B (USUAL CANDY)" << endl;
				GameObject*	GO2	=	NULL;
				GO2	=	new GameObject(); //	<---- Change to Candy when implemented!!!!!!!!!
				GO2->MoveTo(X, 2, Y);

				//	Add candy to both lists
				stationaryObjects->push_back(GO2);
				allGameObjects->push_back(GO2);
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