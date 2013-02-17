#ifndef CAMERA_H
#define CAMERA_H	
#include "stdafx.h"
#include "GameObject.h"

enum CameraState
{
	Follow,
	Free
};

class Camera
{
private:
	CameraState	gState;

	D3DXVECTOR3	gPosition;
	D3DXVECTOR3	gForward, gUp, gRight;
	float		gOffset;

	GameObject	gTarget;

	float		gFoV;
	float		gAspectRatio;
	float		gNear, gFar;

public:
	Camera(float FoV, float AspectRatio, float Near, float Far);
	~Camera(void);

	void	Update(float deltaTime);
	void	ChangeState(CameraState State);

	void	SetTarget(GameObject Target);

	D3DXMATRIX	GetViewMatrix();
	D3DXMATRIX	GetProjectionMatrix();
};

#endif