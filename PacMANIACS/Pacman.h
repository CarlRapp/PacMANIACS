#pragma once
#ifndef PACMAN_H
#define PACMAN_H
#include "GameObject.h"

class Pacman : public GameObject
{
private:
	float	points;
	bool	TryToMove(D3DXVECTOR3 front, vector<D3DXVECTOR3> availableMoves);

	float	gCooldown;
public:
	Pacman(void);
	~Pacman(void);

	enum NextMove
	{
		Forward, Back, Left, Right
	};

	NextMove gNextMove;

	string	GetName();
	string	GetTextureName();
	float	GetHitRadius();
	float	GetPoints();
	float	GetSpeed();
	void	AddPoints(int points);
	void	SetDestination(float x, float y, float z);
	void	SetDestination(D3DXVECTOR3 Pos);

	void	CalculateMove(vector<D3DXVECTOR3> availableMoves);
	void	CherryMode(float CherryTime);
	bool	InCherryMode();
	void	Update(float deltaTime);
};
#endif