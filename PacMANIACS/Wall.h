#pragma once
#ifndef WALL_H
#define WALL_H

#include "GameObject.h"

class Wall : public GameObject
{

public:
	Wall();
	~Wall();

	string	GetName();
	string	GetTextureName();
};

#endif