#ifndef GOMANAGER_H
#define GOMANAGER_H


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "GameObject.h"
#include "MapManager.h"
#include "Ghost.h"
#include "Candy.h"
#include "Wall.h"
#include "Floor.h"

using namespace std;

class GameObjectManager
{
private:
	vector<GameObject*>*	allGameObjects;
	vector<GameObject*>*	moveableObjects;
	vector<GameObject*>*	stationaryObjects;

	float	mapScale;
	int		mapWidth;
	int		mapHeight;
	char**	gMap;

	void		StartConvert(MapManager* MapData);
	GameObject*	ConvertStringToGameObject(string GameObjectName);

	void		HandleCollision(GameObject* GoA, GameObject* GoB);

	bool		TileIsCrossing(D3DXVECTOR3 Pos);

	bool		IsFloor(int X, int Y);

public:
	GameObjectManager(MapManager* MapData);
	~GameObjectManager();


	void Update(float deltaTime);

	vector<GameObject*>*	GetGameObjects();
};

#endif