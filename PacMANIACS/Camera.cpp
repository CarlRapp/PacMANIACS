#include "Camera.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

Camera::Camera(float FoV, float AspectRatio, float Near, float Far)
{
	gFoV			=	FoV;
	gAspectRatio	=	AspectRatio;
	gNear			=	Near;
	gFar			=	Far;
	gState			=	CameraState::Free;
	gPosition		=	D3DXVECTOR3(0, 0, 0);
	gForward		=	D3DXVECTOR3(0, 0, -1);
	oldMousePos		=	D3DXVECTOR2(-1, -1);

	D3DXVec3Normalize(&(gForward), &D3DXVECTOR3(gForward.x, 0 , gForward.z));
	gUp	=	D3DXVECTOR3(0, 1, 0);
	D3DXVec3Cross(&(gRight), &gUp, &(gForward));

}

void Camera::SetInputManager(InputManager* Input)
{
	gInput	=	Input;
}

void Camera::Update(float deltaTime)
{
	if(gState == CameraState::Follow)
		UpdateFollow(deltaTime);
	else if(gState == CameraState::Free)
		UpdateFree(deltaTime);
}

void Camera::UpdateFollow(float deltaTime)
{
	if(gTarget == NULL)
		return;

	D3DXMATRIX rotation	=	gTarget->GetRotationMatrix();

	D3DXVec3TransformCoord(&gRight, &D3DXVECTOR3(-1,0,0), &rotation);
	D3DXVec3TransformCoord(&gForward, &D3DXVECTOR3(0, 0, -1), &rotation);

	D3DXVec3Cross(&D3DXVECTOR3(0, 1, 0), &gForward, &gRight);
	D3DXVec3Normalize(&(gForward), &D3DXVECTOR3(gForward.x, 0 , gForward.z));

	gPosition.x	=	gTarget->GetPosition().x - gForward.x * 5;
	gPosition.y	=	gTarget->GetPosition().y + 4;
	gPosition.z	=	gTarget->GetPosition().z - gForward.z * 5;
}

void Camera::UpdateFree(float deltaTime)
{
	if(gInput == NULL)
		return;

	if (oldMousePos == D3DXVECTOR2(-1, -1))
	{
		LPPOINT mouseCoords = new POINT();
		GetCursorPos(mouseCoords);
		oldMousePos		=	D3DXVECTOR2(mouseCoords->x, mouseCoords->y);
	}

	float	speed	=	24.0f * deltaTime;
	float	sens	=	0.0016f;

	if (gInput->IsKeyDown(VK_LSHIFT))
		speed *= 2;
	if (gInput->IsKeyDown(VK_SPACE))
		speed *= 2;

	LPPOINT mouseCoords = new POINT();
	GetCursorPos(mouseCoords);

	D3DXVECTOR2	mousePos		=	D3DXVECTOR2(mouseCoords->x, mouseCoords->y);
	D3DXVECTOR2	mouseMovement	=	D3DXVECTOR2(mousePos.x - oldMousePos.x, mousePos.y - oldMousePos.y);

	
	float	rotRight	=	mouseMovement.x * sens;
	float	rotUp		=	mouseMovement.y * sens;

	if (gInput->IsKeyDown('W'))
		gPosition += speed * gForward;
	if (gInput->IsKeyDown('S'))
		gPosition -= speed * gForward;
	if (gInput->IsKeyDown('D'))
		gPosition += speed * gRight;
	if (gInput->IsKeyDown('A'))
		gPosition -= speed * gRight;

	if (gInput->IsKeyDown('E'))
		gPosition += speed * gUp;
	if (gInput->IsKeyDown('Q'))
		gPosition -= speed * gUp;

	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation, rotRight);
	D3DXVec3TransformCoord(&gRight, &gRight, &rotation);
	D3DXVec3TransformCoord(&gForward, &gForward, &rotation);

	D3DXMatrixRotationAxis(&rotation, &gRight, rotUp);
	D3DXVec3TransformCoord(&gForward, &gForward, &rotation);

	D3DXVec3Cross(&gUp, &gForward, &gRight);

	oldMousePos	=	mousePos;
}

D3DXMATRIX Camera::GetViewMatrix()
{
	float	pr	=	gPosition.x * gRight.x		+ gPosition.y * gRight.y	+ gPosition.z * gRight.z;
	float	pu	=	gPosition.x * gUp.x			+ gPosition.y * gUp.y		+ gPosition.z * gUp.z;
	float	pf	=	gPosition.x * gForward.x	+ gPosition.y * gForward.y	+ gPosition.z * gForward.z;
	

	return D3DXMATRIX(	gRight.x	,	gUp.x	, gForward.x	, 0,
						gRight.y	,	gUp.y	, gForward.y	, 0,
						gRight.z	,	gUp.z	, gForward.z	, 0,
						-pr			,	-pu		, -pf			, 1);
}

D3DXMATRIX Camera::GetProjectionMatrix()
{
	D3DXMATRIX projcetion;
	D3DXMatrixPerspectiveFovLH(&projcetion, gFoV, gAspectRatio, gNear, gFar);

	return projcetion;
}

void Camera::SetTarget(GameObject* Target)
{
	gState	=	(Target == NULL) ? CameraState::Free : CameraState::Follow;

	gUp	=	D3DXVECTOR3(0, 1, 0);
	gTarget	=	Target;

	if(gTarget != NULL)
	{
		D3DXMATRIX rotation	=	gTarget->GetRotationMatrix();

		D3DXVec3TransformCoord(&gRight, &D3DXVECTOR3(-1,0,0), &rotation);
		D3DXVec3TransformCoord(&gForward, &D3DXVECTOR3(0, 0, -1), &rotation);

		D3DXVec3Cross(&(gRight), &gUp, &(gForward));

		gPosition.x	=	gTarget->GetPosition().x - gForward.x * 5;
		gPosition.y	=	gTarget->GetPosition().y + 4;
		gPosition.z	=	gTarget->GetPosition().z - gForward.z * 5;
	}
}

D3DXVECTOR3 Camera::GetPosition()
{
	return gPosition;
}

D3DXVECTOR3 Camera::GetForward()
{
	return gForward;
}