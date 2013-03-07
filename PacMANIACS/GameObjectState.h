#pragma once
#ifndef GAMEOBJECTSTATE_H
#define GAMEOBJECTSTATE_H	

#include <string>

using namespace std;

class GameObjectState
{
public:
	virtual bool IsAlive() = 0;
};

class AliveGameObjectState : public GameObjectState
{
public:
	bool IsAlive();
};

class DeadGameObjectState : public GameObjectState
{
public:
	bool IsAlive();
};

class IdleGameObjectState : public GameObjectState
{
public:
	bool IsAlive();
};

#endif
