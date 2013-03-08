#include "Cherry.h"


Cherry::Cherry(void) : GameObject()
{
	gTimeLived		=	0.0f;
}


Cherry::~Cherry(void)
{

}

string Cherry::GetName()
{
	return "Cherry";
}

string Cherry::GetTextureName()
{
	return "Cherry_Texture.png";
}

float Cherry::GetHitRadius()
{
	return 0.4f;
}

bool Cherry::IsStationary()
{
	return true;
}

void Cherry::Update(float deltaTime)
{
	if(gTimeLived == 0)
	{
		gStartPosition	=	GetPosition();
		gTimeLived		+=	rand();
	}

	GameObject::Update(deltaTime);

	SetRotation(0, gTimeLived, 0);

	SetPosition(gStartPosition.x, gStartPosition.y + sin(8 * gTimeLived) * 0.3f, gStartPosition.z);

	gTimeLived	+=	deltaTime;
}