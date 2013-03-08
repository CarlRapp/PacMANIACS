#pragma once
#ifndef PACMAN_H
#define PACMAN_H
#include "GameObject.h"

class Pacman : public GameObject
{
private:
	float	points;
public:
	Pacman(void);
	~Pacman(void);

	string	GetName();
	string	GetTextureName();
	float	GetHitRadius();
	float	GetPoints();
	void	AddPoints(int points);

};
#endif