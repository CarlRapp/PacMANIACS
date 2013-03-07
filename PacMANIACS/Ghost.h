#pragma once
#ifndef GHOST_H
#define GHOST_H	

#include "GameObject.h"

#include "GhostAIState.h"

class Ghost : public GameObject
{
	GhostAIState* gAIState;

public:
	Ghost(void);
	~Ghost(void);

	string	GetName();
	string	GetTextureName();
	float	GetHitRadius();

	void	SetAIState(GhostAIState* AIState);
};

#endif
