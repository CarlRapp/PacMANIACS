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
	return "Candy_Texture.png";
}

float Candy::GetHitRadius()
{
	return 5.0f;
}
