#pragma once
#ifndef CANDY_H
#define CANDY_H	

#include "GameObject.h"

class Candy : public GameObject
{
	float		gTimeLived;
	D3DXVECTOR3	gStartPosition;

public:
	Candy(void);
	~Candy(void);

	string	GetName();
	string	GetTextureName();
	float	GetHitRadius();
	bool	IsStationary();

	void	Update(float deltaTime);
};

#endif
