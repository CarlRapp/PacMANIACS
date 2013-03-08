#include "Pacman.h"

Pacman::Pacman(){
};

Pacman::~Pacman(){
};

string Pacman::GetName(){
	return "Pacman";
};

string Pacman::GetTextureName(){
	return "PacMan_Texture.png";
};

float Pacman::GetHitRadius(){
	return 3.0f;
};

float Pacman::GetPoints(){
	return points;
};

void Pacman::AddPoints(int points){
	Pacman::points += points;
};