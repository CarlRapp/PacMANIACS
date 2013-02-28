#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(MapManager* MapData)
{
	gMap	=	MapData->GetMap();


	StartConvert(MapData);
}

void GameObjectManager::StartConvert(MapManager* MapData)
{
	moveableObjects		=	new vector<GameObject*>();
	stationaryObjects	=	new vector<GameObject*>();

	int		mWidth	=	MapData->GetWidth();
	int		mHeight	=	MapData->GetHeight();

	for(int Y = 0; Y < mHeight; ++Y)
	{
		for(int X = 0; X < mWidth; ++X)
		{
			char	currentChar	=	gMap[X][Y];

			if(MapData->GetGameObjectList().count(currentChar) > 0)
			{
				string	ObjectName	=	MapData->GetGameObjectList().at(currentChar);

				GameObject*	GO	=	ConvertStringToGameObject(ObjectName);
				GO->MoveTo(X, 2, Y);

				if(GO->IsStationary())
					stationaryObjects->push_back(GO);
				else
				{
					cout << "ADD CANDY A" << endl;
					moveableObjects->push_back(GO);
				}
			}
			else if (currentChar == '0')
			{
				cout << "ADD CANDY B" << endl;
			}
		}
	}
}

GameObject* GameObjectManager::ConvertStringToGameObject(string GOName)
{
	GameObject*	GO;

	if(GOName == "Pacman")
	{
		GO	=	new GameObject();
	}
	else if(GOName == "Ghost1")
	{
		GO	=	new GameObject();
	}
	else if(GOName == "Ghost2")
	{

	}
	else if(GOName == "Ghost3")
	{

	}

	return GO;
}