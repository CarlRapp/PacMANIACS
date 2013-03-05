#ifndef GOMANAGER_H
#define GOMANAGER_H


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "GameObject.h"
#include "MapManager.h"

using namespace std;

class GameObjectManager
{
private:
	vector<GameObject*>*	allGameObjects;
	vector<GameObject*>*	moveableObjects;
	vector<GameObject*>*	stationaryObjects;
	char**	gMap;

	void		StartConvert(MapManager* MapData);
	GameObject*	ConvertStringToGameObject(string GameObjectName);

	void		HandleCollision(GameObject* GoA, GameObject* GoB);

public:
	GameObjectManager(MapManager* MapData);
	~GameObjectManager();


	void Update(float deltaTime);

	vector<GameObject*>*	GetGameObjects();
};

#endif