#include "Candy.h"


Candy::Candy(void) : GameObject()
{
	gTimeLived		=	0.0f;
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
	return RescaleHitRadius(0.395);
}

bool Candy::IsStationary()
{
	return true;
}

void Candy::Update(float deltaTime)
{
	if(gTimeLived == 0)
	{
		gStartPosition	=	GetPosition();
		gTimeLived		+=	rand();
	}

	GameObject::Update(deltaTime);

	SetPosition(gStartPosition.x + sin(4 * gTimeLived) * 0.2f, gStartPosition.y + sin(8 * gTimeLived) * 0.3f, gStartPosition.z + cos(4 * gTimeLived) * 0.2f - 0.1f);

	gTimeLived	+=	deltaTime;
}