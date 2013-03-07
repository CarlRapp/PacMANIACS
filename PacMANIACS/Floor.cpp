#include "Floor.h"


Floor::Floor() : GameObject()
{

}

Floor::~Floor()
{

}


string Floor::GetName()
{
	return "Floor";
}

string Floor::GetTextureName()
{
	return "Floor.png";
}