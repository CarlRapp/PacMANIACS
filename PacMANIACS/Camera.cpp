#include "Camera.h"

Camera::Camera(D3DXVECTOR3 Position, D3DXVECTOR3 Forward, float Offset)
{	
	offset = Offset;

	Camera::Position = Position;
	state = CameraState::WalkMode;
	D3DXVec3Normalize(&(Camera::Forward), &D3DXVECTOR3(Forward.x, 0 , Forward.z));

	Up = D3DXVECTOR3(0, 1, 0);

	D3DXVec3Cross(&(Camera::Right), &Up, &(Camera::Forward));
}

float sens = 0.0016;
void Camera::Update(float deltaTime,  D3DXVECTOR2 mouseMovement)
{
	float rotUp = 0, rotRight = 0;

	rotRight	+= sens * mouseMovement.x;
	rotUp		-= sens * mouseMovement.y;

	float speed = 24.0f * deltaTime;
	if (GetAsyncKeyState(VK_LSHIFT))
		speed *= 2;
	if (GetAsyncKeyState(VK_SPACE))
		speed *= 2;
	
	if (GetAsyncKeyState('W'))
		Position += speed * Forward;
	if (GetAsyncKeyState('S'))
		Position -= speed * Forward;
	if (GetAsyncKeyState('D'))
		Position += speed * Right;
	if (GetAsyncKeyState('A'))
		Position -= speed * Right;

	if (GetAsyncKeyState('E'))
		Position += speed * Up;
	if (GetAsyncKeyState('Q'))
		Position -= speed * Up;

	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation, rotRight);
	D3DXVec3TransformCoord(&Right, &Right, &rotation);
	D3DXVec3TransformCoord(&Forward, &Forward, &rotation);
	
	D3DXMatrixRotationAxis(&rotation, &Right, rotUp);
	D3DXVec3TransformCoord(&Forward, &Forward, &rotation);

	D3DXVec3Cross(&Up, &Forward, &Right);
}

D3DXMATRIX Camera::GetViewMatrix()
{
	float pr = Position.x * Right.x		+ Position.y * Right.y		+ Position.z * Right.z;
	float pu = Position.x * Up.x		+ Position.y * Up.y			+ Position.z * Up.z;
	float pf = Position.x * Forward.x	+ Position.y * Forward.y	+ Position.z * Forward.z;
	
	return D3DXMATRIX(	Right.x			, Up.x				, Forward.x			, 0,
						Right.y			, Up.y				, Forward.y			, 0,
						Right.z			, Up.z				, Forward.z			, 0,
						-pr				, -pu				, -pf				, 1);						
}

D3DXMATRIX Camera::GetProjectionMatrix(float FovY, float aspectRatio, float nearLimit, float farLimit)
{
	D3DXMATRIX projcetion;
	D3DXMatrixPerspectiveFovLH(&projcetion, FovY, aspectRatio, nearLimit, farLimit);
	return projcetion;
}

float Camera::GetOffset()
{
	return offset;
}