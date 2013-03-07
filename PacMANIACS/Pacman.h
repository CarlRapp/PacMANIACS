#pragma once
#ifndef PACMAN_H
#define PACMAN_H
#include "GameObject.h"

class Pacman : public GameObject
{
private:

public:
	Pacman(void);
	~Pacman(void);

	string	GetName();
	string	GetTextureName();
};

#endif