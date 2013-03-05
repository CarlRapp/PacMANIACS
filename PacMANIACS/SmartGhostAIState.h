#pragma once
#ifndef SMARTGHOSTAISTATE_H
#define SMARTGHOSTAISTATE_H	

#include "GhostAIState.h"

class SmartGhostAIState : public GhostAIState
{
public:
	SmartGhostAIState(void);
	~SmartGhostAIState(void);
	string GetTextureName();
};

#endif