#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(MapManager* MapData)
{
	mapScale	=	3.0f;
	gMap		=	MapData->GetMap();


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
			int	xPos	=	mapScale * X;
			int	yPos	=	-mapScale * Y;

			if(MapData->GetGameObjectList().count(currentChar) > 0)
			{
				string	ObjectName	=	MapData->GetGameObjectList().at(currentChar);
				GameObject*	GO	=	NULL;
				GO	=	ConvertStringToGameObject(ObjectName);
				GO->SetScale(0.5f, 0.5f, 0.5f);
				GO->MoveTo(xPos, 1, yPos);
				
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
					GO2->MoveTo(xPos, 1, yPos);

					//	Add candy to both lists
				//	stationaryObjects->push_back(GO2);
				//	allGameObjects->push_back(GO2);
				}
			}
			else if (currentChar == '0')
			{
				GameObject*	GO2	=	new Candy();
				GO2->SetScale(0.5f, 0.5f, 0.5f);
				GO2->MoveTo(xPos, 1, yPos);

				//	Add candy to both lists
				stationaryObjects->push_back(GO2);
				allGameObjects->push_back(GO2);
			}


			if(currentChar == '1')
			{
				GameObject*	WALL	=	new Wall();
				WALL->MoveTo(xPos, 0, yPos);
				WALL->SetScale(3, 1, 3);
				allGameObjects->push_back(WALL); 
			}
			else
			{
				GameObject* FLOOR = new Floor();
				FLOOR->MoveTo(xPos, 0, yPos);
				FLOOR->SetScale(3, 3, 3);
				allGameObjects->push_back(FLOOR);
			}
		}
	}

	mapWidth	=	mWidth;
	mapHeight	=	mHeight;
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
	system("cls");
	GameObject*	tPacman	=	allGameObjects->at(0);

	int X, Z;
	X	=	(tPacman->GetPosition().x + mapScale * 0.5f) / mapScale;
	Z	=	-(tPacman->GetPosition().z - mapScale * 0.5f) / mapScale;

	cout << X << ", " << Z << endl;



	

	//cout << tPacman->GetPosition().x << " X" << endl;
	//cout << tPacman->GetPosition().z << " Z" << endl;

	for(int n = 0; n < mapHeight; n++)// Y
	{
		for(int i = 0; i < mapWidth; i++)// X
		{
			if(i == X && n == Z)
				cout << "X";
			else
				cout << gMap[i][n];
		}

		cout << endl;
	}
	if(TileIsCrossing(D3DXVECTOR3(X, 0, Z)))
		cout << "NU JÄVLAR KORSAR JAG" << endl;

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
			{
				HandleCollision(tPacman, A);
			}
	}

	for(int i = stationaryObjects->size() - 1; i >= 0; --i)
	{
		GameObject*	A	=	stationaryObjects->at(i);

		A->Update(deltaTime);

		if(tPacman != NULL)
			if(A->IsColliding(tPacman))
			{
				HandleCollision(tPacman, A);
				for(int n = 0; n < allGameObjects->size(); ++n)
					if(allGameObjects->at(n) == A)
					{
						allGameObjects->erase(allGameObjects->begin() + n);
						break;
					}
				stationaryObjects->erase(stationaryObjects->begin() + i);
			}
	}
}

void GameObjectManager::HandleCollision(GameObject* GoA, GameObject* GoB)
{
	if(GoA->GetName() == "Ghost" && GoB->GetName() == "Candy")
	{

	}


}

vector<GameObject*>* GameObjectManager::GetGameObjects()
{
	return allGameObjects;
}

bool GameObjectManager::TileIsCrossing(D3DXVECTOR3 Pos)
{
	int X, Y, Z;
	X	=	Pos.x;
	Y	=	Pos.y;
	Z	=	Pos.z;

	int	waysToGo	=	0;
	waysToGo		+=	IsFloor(X - 1, Z) ? 1 : 0;
	waysToGo		+=	IsFloor(X + 1, Z) ? 1 : 0;
	waysToGo		+=	IsFloor(X, Z - 1) ? 1 : 0;
	waysToGo		+=	IsFloor(X, Z + 1) ? 1 : 0;

	return (waysToGo > 2);
}

bool GameObjectManager::IsFloor(int X, int Y)
{
	if(X >= 0 && X < mapWidth)
		if(Y >= 0 && Y < mapHeight)
			if(gMap[X][Y] != '1')
				return true;


	return false;
}