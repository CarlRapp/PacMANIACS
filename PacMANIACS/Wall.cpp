#include "Wall.h"


Wall::Wall() : GameObject()
{

}

Wall::~Wall()
{

}


string Wall::GetName()
{
	return "Wall";
}

string Wall::GetTextureName()
{
	return "Wall.png";
}

float Wall::GetHitRadius()
{
	return 1.0f;
}