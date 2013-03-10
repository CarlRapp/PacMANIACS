#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

const string MAPNAME = "../Data/original.txt";

const string MAPTOKEN = "#";
const string GAMEOBJECTTOKEN = "-";

enum Token
{
	Map,
	GameEntity,
	Undefined
};

class MapManager
{
private:
	map<char, string> m_gameObjectList;
	char** m_map;
	int m_mapWidth;
	int m_mapHeight;
	

	void LoadMap();
	Token GetToken(std::string);
public:
	MapManager();
	~MapManager();

	char** GetMap();
	map<char, string> GetGameObjectList();
	int GetWidth();
	int GetHeight();
};