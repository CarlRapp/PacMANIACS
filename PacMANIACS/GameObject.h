#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H	

#include "stdafx.h"
#include "GameObjectState.h"

#include <string>

using namespace std;


class GameObject
{
private:
	//	Fields
	GameObjectState* gState;

	//	Matrices
	D3DXMATRIX	gRotation;
	D3DXMATRIX	gScale;
	D3DXMATRIX	gTranslation;
	D3DXMATRIX	gWorld;
	D3DXMATRIX	gWorldInverseTranspose;
	D3DXVECTOR3	gRotationFloat;

	D3DXVECTOR3	gVelocity;
	D3DXVECTOR3	gTargetPosition;


	bool		gReadyForMove;


	//	Private methods
	void	UpdateWorldMatrix(bool UpdateInvTrans);

protected:
	virtual	float	GetSpeed();
	float			RescaleHitRadius(float radius);

public:
	GameObject(void);
	~GameObject(void);

	virtual void Update(float deltaTime);

	//	Get methods
	D3DXMATRIX		GetWorldMatrix(void);
	D3DXMATRIX		GetWorldInverseTranspose(void);
	D3DXMATRIX		GetRotationMatrix(void);
	D3DXVECTOR3		GetPosition(void);

	virtual string	GetName();
	virtual string	GetTextureName();

	virtual bool	IsStationary();
	virtual	float	GetHitRadius();

	bool	IsAlive();

	//	Set methods
	void	SetRotation(float x, float y, float z);
	void	AddRotation(float dX, float dY, float dZ);

	void	SetScale(float x, float y, float z);

	void	Move(D3DXVECTOR3 pos);
	void	SetVelocity(float x, float y, float z);
	void	SetVelocity(D3DXVECTOR3 Vel);

	void	Move(float dX, float dY, float dZ);
	void	MoveTo(D3DXVECTOR3 pos);
	void	MoveTo(float x, float y, float z);

	void	SetPosition(float x, float y, float z);

	void	SetLook(D3DXVECTOR2 direction);

	bool	IsColliding(GameObject* GO);

	bool	AtDestination(void);
	void	virtual SetDestination(float x, float y, float z);
	void	virtual SetDestination(D3DXVECTOR3 Pos);

	D3DXVECTOR3 GetVelocity(void);
};

#endif