#pragma once
#ifndef GHOST_H
#define GHOST_H	

#include "GameObject.h"

#include "GhostAIState.h"
#include "StupidGhostAIState.h"
#include "NormalGhostAIState.h"
#include "SmartGhostAIState.h"
#include "GeniusGhostAIState.h"

class Ghost : public GameObject
{
	GhostAIState* gAIState;

public:
	Ghost(void);
	~Ghost(void);

	string	GetName();
	string	GetTextureName();
	
	void	SetAIState(GhostAIState* AIState);
};

#endif
