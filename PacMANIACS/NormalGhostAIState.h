#pragma once
#ifndef NORMALGHOSTAISTATE_H
#define NORMALGHOSTAISTATE_H	

#include "GhostAIState.h"

class NormalGhostAIState : public GhostAIState
{
public:
	NormalGhostAIState(void);
	virtual ~NormalGhostAIState(void);
	string GetTextureName();
};

#endif