#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H	

#include "stdafx.h"
#include <string>

using namespace std;

enum GameObjectState
{
	Alive,
	Idle,
	Dead
};

class GameObject
{
private:
	//	Fields
	GameObjectState	gState;

	//	Matrices
	D3DXMATRIX	gRotation;
	D3DXMATRIX	gScale;
	D3DXMATRIX	gTranslation;
	D3DXMATRIX	gWorld;
	D3DXMATRIX	gWorldInverseTranspose;



	//	Private methods
	void	UpdateWorldMatrix(bool UpdateInvTrans);

public:
	GameObject(void);
	~GameObject(void);

	void Update(float deltaTime);

	//	Get methods
	D3DXMATRIX		GetWorldMatrix(void);
	D3DXMATRIX		GetWorldInverseTranspose(void);
	D3DXMATRIX		GetRotationMatrix(void);
	D3DXVECTOR3		GetPosition(void);

	virtual string	GetName();
	virtual bool	IsStationary();
	virtual	float	GetHitRadius();

	bool	IsAlive();

	//	Set methods
	void	SetRotation(float x, float y, float z);
	void	SetScale(float x, float y, float z);
	void	Move(float dX, float dY, float dZ);
	void	MoveTo(float x, float y, float z);

	
	bool	IsColliding(GameObject* GO);
};

#endif