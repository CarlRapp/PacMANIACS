#include "GameObject.h"




GameObject::GameObject()
{
	gRotationFloat	=	D3DXVECTOR3(0, 0, 0);
	SetVelocity(0, 0, 0);
	MoveTo(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScale(1, 1, 1);

	gState			=	new AliveGameObjectState();

	gTargetPosition	=	D3DXVECTOR3(0, 0, 0);
	gReadyForMove	=	false;
}

GameObject::~GameObject()
{

}

void GameObject::Update(float deltaTime)
{
	gReadyForMove	=	false;

	Move(gVelocity.x * deltaTime, gVelocity.y * deltaTime, gVelocity.z * deltaTime);

	D3DXVECTOR3	v1	=	gTargetPosition - GetPosition();
	if(D3DXVec3Dot(&v1, &gVelocity) < 0)
	{
		SetPosition(gTargetPosition.x, gTargetPosition.y, gTargetPosition.z);
		gReadyForMove	=	true;
	}
}

bool GameObject::AtDestination()
{
	return gReadyForMove;
}

bool GameObject::IsAlive()
{
	return gState->IsAlive();
}

void GameObject::SetVelocity(float x, float y, float z)
{
	gVelocity.x	=	x;
	gVelocity.y =	y;
	gVelocity.z	=	z;
}

void GameObject::SetVelocity(D3DXVECTOR3 Vel)
{
	SetVelocity(Vel.x, Vel.y, Vel.z);
}

void GameObject::SetDestination(float x, float y, float z)
{
	gVelocity.x			=	x - GetPosition().x;
	gVelocity.y			=	0;
	gVelocity.z			=	z - GetPosition().z;
	D3DXVec3Normalize(&gVelocity, &gVelocity);
	gVelocity			*=	GetSpeed();

	gTargetPosition.x	=	x;
	gTargetPosition.y	=	GetPosition().y;
	gTargetPosition.z	=	z;
}

void GameObject::SetDestination(D3DXVECTOR3 Pos)
{
	SetDestination(Pos.x, Pos.y, Pos.z);
}

void GameObject::SetRotation(float x, float y, float z)
{
	D3DXMatrixRotationYawPitchRoll(&gRotation, y, x, z);
	gRotationFloat.x	=	x;
	gRotationFloat.y	=	y;
	gRotationFloat.z	=	z;

	UpdateWorldMatrix(true);
}

void GameObject::AddRotation(float dx, float dy, float dz)
{
	gRotationFloat.x	+=	dx;
	gRotationFloat.y	+=	dy;
	gRotationFloat.z	+=	dz;

	D3DXMatrixRotationYawPitchRoll(&gRotation, gRotationFloat.y, gRotationFloat.x, gRotationFloat.z);

	UpdateWorldMatrix(true);
}

void GameObject::SetScale(float x, float y, float z)
{
	D3DXMatrixScaling(&gScale, x, y, z);
	UpdateWorldMatrix(true);
}

void GameObject::Move(D3DXVECTOR3 pos)
{
	Move(pos.x, pos.y, pos.z);
}

void GameObject::Move(float dx, float dy, float dz)
{
	gTranslation._41	+=	dx;
	gTranslation._42	+=	dy;
	gTranslation._43	+=	dz;

	UpdateWorldMatrix(false);
}

void GameObject::MoveTo(D3DXVECTOR3 pos)
{
	MoveTo(pos.x, pos.y, pos.z);
}

void GameObject::MoveTo(float x, float y, float z)
{
	D3DXMatrixTranslation(&gTranslation, x, y, z);

	UpdateWorldMatrix(false);
}

void GameObject::SetPosition(float x, float y, float z)
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

bool GameObject::IsStationary()
{
	return false;
}

float GameObject::GetHitRadius()
{
	return 0.0f;
}

float GameObject::GetSpeed()
{
	return 2.0f;
}

bool GameObject::IsColliding(GameObject* GO)
{
	D3DXVECTOR3*		p1	=	&GetPosition();
	D3DXVECTOR3*		p2	=	&GO->GetPosition();
	D3DXVECTOR3*		p3	=	&(*p2 - *p1);

	float	LengthBetween	=	D3DXVec3Length(p3);

	return (LengthBetween < GetHitRadius() + GO->GetHitRadius());
}

D3DXVECTOR3 GameObject::GetPosition()
{
	return D3DXVECTOR3(gTranslation._41, gTranslation._42, gTranslation._43);
}

D3DXVECTOR3 GameObject::GetVelocity()
{
	return gVelocity;
}