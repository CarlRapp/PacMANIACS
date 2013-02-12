#ifndef CAMERA_H
#define CAMERA_H	
#include "stdafx.h"
class Camera
{


public:
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Forward;
	D3DXVECTOR3 Up;
	D3DXVECTOR3 Right;
	float offset;

	float VelocityY;

	enum CameraState { WalkMode, FreeLookMode };
	CameraState state;

	Camera(D3DXVECTOR3 Position, D3DXVECTOR3 Forward, float Offset);
	void Update(float deltaTime, D3DXVECTOR2 ViewProjection);
	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjectionMatrix(float FovY, float aspectRatio, float nearLimit, float farLimit);	
	float GetOffset();

};
#endif