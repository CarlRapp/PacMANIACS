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
	return "Ghost_Purpel_Texture.png";
}

float Cherry::GetHitRadius()
{
	return 0.4f;
}

void Cherry::Update(float deltaTime)
{
	if(gTimeLived == 0)
	{
		gTimeLived		+=	rand();
	}

	GameObject::Update(deltaTime);

	SetRotation(0, gTimeLived, 0);

	gTimeLived	+=	deltaTime;
}