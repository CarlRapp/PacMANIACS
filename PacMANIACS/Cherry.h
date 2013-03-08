#pragma once
#ifndef CHERRY_H
#define CHERRY_H	

#include "GameObject.h"

class Cherry : public GameObject
{
	float		gTimeLived;

public:
	Cherry(void);
	~Cherry(void);

	string	GetName();
	string	GetTextureName();
	float	GetHitRadius();

	void	Update(float deltaTime);
};
#endif