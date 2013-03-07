#pragma once
#ifndef FLOOR_H
#define FLOOR_H	

#include "GameObject.h"

class Floor : public GameObject
{

public:
	Floor();
	~Floor();

	string	GetName();
	string	GetTextureName();
};

#endif