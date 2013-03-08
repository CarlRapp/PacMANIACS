#pragma once
#ifndef CHERRY_H
#define CHERRY_H	

#include "GameObject.h"

class Cherry : public GameObject
{
	float		gTimeLived;
	D3DXVECTOR3	gStartPosition;

public:
	Cherry(void);
	~Cherry(void);

	string	GetName();
	string	GetTextureName();
	float	GetHitRadius();
	bool	IsStationary();

	void	Update(float deltaTime);
};
#endif