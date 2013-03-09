#ifndef GOMANAGER_H
#define GOMANAGER_H


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "GameObject.h"
#include "MapManager.h"
#include "SoundManager.h"
#include "Ghost.h"
#include "Candy.h"
#include "Wall.h"
#include "Floor.h"
#include "Pacman.h"
#include "Cherry.h"

using namespace std;

class GameObjectManager
{
private:
	vector<GameObject*>*	allGameObjects;
	vector<GameObject*>*	moveableObjects;
	vector<GameObject*>*	stationaryObjects;

	SoundManager*	gSoundManager;
	float			mapScale;
	int				mapWidth;
	int				mapHeight;
	char**			gMap;

	void		StartConvert(MapManager* MapData);
	GameObject*	ConvertStringToGameObject(string GameObjectName);

	void		HandleCollision(GameObject* GoA, GameObject* GoB);

	bool		IsTileCrossing(int X, int Z);
	bool		IsTileCorner(int X, int Z);
	bool		IsFloor(int X, int Z);
	void		AlertGhosts(GameObject* CherryEater);

public:
	GameObjectManager(MapManager* MapData, SoundManager* soundManager);
	~GameObjectManager();

	void Update(float deltaTime);

	vector<GameObject*>*	GetGameObjects();

	D3DXVECTOR2	GetTilePosition(D3DXVECTOR3 Pos);
	D3DXVECTOR3	GetWorldPosition(int X, int Y, int Z);

	vector<D3DXVECTOR3>	GetAvailableMoves(int X, int Z);
};

#endif