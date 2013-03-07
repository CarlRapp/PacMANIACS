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

class StupidGhostAIState : public GhostAIState
{
public:
	string GetTextureName();
};

class NormalGhostAIState : public GhostAIState
{
public:
	string GetTextureName();
};

class SmartGhostAIState : public GhostAIState
{
public:
	string GetTextureName();
};

class GeniusGhostAIState : public GhostAIState
{
public:
	string GetTextureName();
};

#endif
