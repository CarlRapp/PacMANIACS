#pragma once
#ifndef GHOSTAISTATE_H
#define GHOSTAISTATE_H	

#include <string>

using namespace std;

class GhostAIState
{
public:
	virtual string GetTextureName() = 0;
};

#endif
