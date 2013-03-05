#pragma once
#ifndef GHOSTAISTATE_H
#define GHOSTAISTATE_H	

#include <string>

using namespace std;

class GhostAIState
{

public:
	GhostAIState(void);
	~GhostAIState(void);
	virtual string GetTextureName() = 0;
};

#endif
