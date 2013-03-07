#include "Candy.h"


Candy::Candy(void) : GameObject()
{

}


Candy::~Candy(void)
{

}

string Candy::GetName()
{
	return "Candy";
}

string Candy::GetTextureName()
{
	return "Candy.png";
}

float Candy::GetHitRadius()
{
	return 0.4f;
}
