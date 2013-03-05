#pragma once
#ifndef GENIUSGHOSTAISTATE_H
#define GENIUSGHOSTAISTATE_H	

#include "GhostAIState.h"

class GeniusGhostAIState : public GhostAIState
{
public:
	GeniusGhostAIState(void);
	~GeniusGhostAIState(void);
	string GetTextureName();
};

#endif