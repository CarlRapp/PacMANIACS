#pragma once
#ifndef STUPIDGHOSTAISTATE_H
#define STUPIDGHOSTAISTATE_H	

#include "GhostAIState.h"

class StupidGhostAIState : public GhostAIState
{
public:
	StupidGhostAIState(void);
	~StupidGhostAIState(void);
	string GetTextureName();
};

#endif