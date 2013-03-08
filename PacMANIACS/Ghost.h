#pragma once
#ifndef GHOST_H
#define GHOST_H	

#include "GameObject.h"

class GhostAIState
{
public:
	virtual string GetTextureName() = 0;
	virtual void CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves) = 0;
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
};

#endif
