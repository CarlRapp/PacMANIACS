#include "GameObject.h"




GameObject::GameObject()
{
	MoveTo(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScale(1, 1, 1);

	gState	=	Alive;
}

GameObject::~GameObject()
{

}

void GameObject::Update(float deltaTime)
{
	Move(1*deltaTime, 1*deltaTime, 1*deltaTime);
}

bool GameObject::IsAlive()
{
	return (gState != Dead);
}


void GameObject::SetRotation(float x, float y, float z)
{
	D3DXMatrixRotationYawPitchRoll(&gRotation, x, y, z);

	UpdateWorldMatrix(true);
}

void GameObject::SetScale(float x, float y, float z)
{
	D3DXMatrixScaling(&gScale, x, y, z);
	UpdateWorldMatrix(true);
}

void GameObject::Move(float dx, float dy, float dz)
{
	gTranslation._41	+=	dx;
	gTranslation._42	+=	dy;
	gTranslation._43	+=	dz;

	UpdateWorldMatrix(false);
}

void GameObject::MoveTo(float x, float y, float z)
{
	D3DXMatrixTranslation(&gTranslation, x, y, z);

	UpdateWorldMatrix(false);
}

void GameObject::UpdateWorldMatrix(bool UpdateInvTrans)
{
	//	Update the world matrix
	gWorld					=	gScale * gRotation * gTranslation;

	//	Abort the method if
	//	we shouldnt update the
	//	inverse transpose
	if(!UpdateInvTrans)
		return;

	gWorldInverseTranspose	=	gScale * gRotation;

	//	We need the determinant to
	//	calculate the inverse transpose
	FLOAT	DET;
	DET	=	D3DXMatrixDeterminant(&gWorldInverseTranspose);
	
	D3DXMatrixInverse(&gWorldInverseTranspose, &DET, &gWorldInverseTranspose);
	D3DXMatrixTranspose(&gWorldInverseTranspose, &gWorldInverseTranspose);
}

D3DXMATRIX GameObject::GetRotationMatrix(void)
{
	return gRotation;
}

D3DXMATRIX GameObject::GetWorldMatrix(void)
{
	return gWorld;
}

D3DXMATRIX GameObject::GetWorldInverseTranspose(void)
{
	return gWorldInverseTranspose;
}

string GameObject::GetName()
{
	return "";
}

string GameObject::GetTextureName()
{
	return "";
}