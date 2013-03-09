#pragma once
#ifndef GHOST_H
#define GHOST_H	

#include "GameObject.h"
#include <iostream>
using namespace std;

class GhostTargetState
{
protected:
	string	gTextureName;

public:
	void SetTextureName(string TextureName);
	virtual	void Update(float deltaTime);
	virtual	string GetTextureName();
};

class FleeState : public GhostTargetState
{
private:
	bool	gDoBlink;
	float	gTimeTicked;
	string	gFleeTexture;

public:
	void Update(float deltaTime);
	void SetFleeTexture(string TextureName);
	string GetTextureName();
};

class GhostAIState
{
	GhostTargetState*	gActiveState;

public:
	string	GetActiveTextureName();
	virtual string GetTextureName() = 0;
	virtual void CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves) = 0;
	void	SetTargetState(GhostTargetState* State);

	void	Update(float deltaTime);
};

class StupidGhostAIState : public GhostAIState
{
public:
	string GetTextureName();
	void CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves);
};

class NormalGhostAIState : public GhostAIState
{
public:
	string GetTextureName();
	void CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves);
};

class SmartGhostAIState : public GhostAIState
{
public:
	string GetTextureName();
	void CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves);
};

class GeniusGhostAIState : public GhostAIState
{
public:
	string GetTextureName();
	void CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves);
};

class Ghost : public GameObject
{
	GhostAIState*	gAIState;
	GameObject*		gTarget;

	GhostTargetState*	gHuntState;
	GhostTargetState*	gFleeState;
	float			gCooldown;

public:
	Ghost(void);
	~Ghost(void);

	string	soundKey;

	string	GetName();
	string	GetTextureName();
	float	GetHitRadius();

	void	SetAIState(GhostAIState* AIState);
	void	CalculateMove(vector<D3DXVECTOR3> availableMoves);
	void	SetTarget(GameObject* Target);
	void	Update(float deltaTime);

	void	HuntTarget(void);
	void	FleeTarget(float CherryTime);
};

#endif
