#include "MapManager.h"



MapManager::MapManager()
{
	LoadMap();
}

MapManager::~MapManager()
{
	for (int x = 0; x < m_mapWidth; x++)
	{
		delete[] m_map[x];
	}
	delete m_map;
}

void MapManager::LoadMap()
{
	m_gameObjectList = map<char, string>();

	vector<string> mapLines = vector<string>();

	ifstream myfile = ifstream();
	myfile.open(MAPNAME, std::ios::out);
	string sLine;

	while (!myfile.eof())
	{
		getline(myfile, sLine);

		if(sLine.length() > 0)
		{
			switch(GetToken(sLine))
			{
				case Token::Map:
					// Store the map lines in a temporary list for use later
					mapLines.push_back(sLine.substr(1, sLine.length() - 1));
					break;

				case Token::GameEntity:
					// Key = the first character after the gameobject token
					// Value = name of the game object
					m_gameObjectList.insert(std::map<char, string>::value_type(sLine.at(1), sLine.substr(3, sLine.length() - 3)));
					break;
			}
		}
	}
	myfile.close();

	// Find the longest line of all the lines
	int longestLine = 0;
	for (int i = 0; i < mapLines.size(); i++)
		if (mapLines[i].length() > longestLine)
			longestLine = mapLines[i].length();
	m_mapWidth = longestLine;
	m_mapHeight = mapLines.size();

	// Initialize the map 2D array and set all values
	m_map = new char*[m_mapWidth];
	for (int x = 0; x < m_mapWidth; x++)
	{
		m_map[x] = new char[m_mapHeight];
		for (int y = 0; y < m_mapHeight; y++)
		{
			// Check the char at the x y position
			char mapChar = mapLines[y].at(x);
			// See if the char exists in the game object list
			map<char,string>::iterator it = m_gameObjectList.find(mapChar);
			// If the char exists in the game object list or if it is a digit then add it to the map array
			if (it != m_gameObjectList.end() | isdigit(mapChar))
				m_map[x][y] = mapChar;
		}
	}
}

Token MapManager::GetToken(std::string line)
{
	// Get the first letter (the token)
	string tokenStr = line.substr(0, 1);

	//	Convert the token to
	//	the correct enum.
	if(tokenStr == MAPTOKEN)
		return Token::Map;

	if(tokenStr == GAMEOBJECTTOKEN)
		return Token::GameEntity;

	//	If there is a undefined token
	//	pass this. (Usually comments/blank rows)
	return Token::Undefined;
}

char** MapManager::GetMap()
{
	return m_map;
}

map<char, string> MapManager::GetGameObjectList()
{
	return m_gameObjectList;
}