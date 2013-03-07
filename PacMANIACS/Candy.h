#pragma once
#ifndef CANDY_H
#define CANDY_H	

#include "GameObject.h"

class Candy : public GameObject
{

public:
	Candy(void);
	~Candy(void);

	string	GetName();
	string	GetTextureName();
	float	GetHitRadius();
};

#endif
