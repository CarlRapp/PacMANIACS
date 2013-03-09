#include "Pacman.h"

Pacman::Pacman()
{
	gNextMove = NextMove::Forward;
	points	=	0;

	gCooldown	=	0;
}

Pacman::~Pacman()
{
}

string Pacman::GetName()
{
	return "Pacman";
}

string Pacman::GetTextureName()
{
	return "PacMan_Texture.png";
}

float Pacman::GetSpeed()
{
	return (gCooldown > 0) ? 6 : GameObject::GetSpeed();
}

float Pacman::GetHitRadius()
{
	return 0.8f;
}

float Pacman::GetPoints()
{
	return points;
}

void Pacman::AddPoints(int points)
{
	Pacman::points += points;
}

void Pacman::CherryMode(float CherryTime)
{
	gCooldown	=	CherryTime;
}

void Pacman::SetDestination(float x, float y, float z)
{
	GameObject::SetDestination(x, y, z);
	gNextMove = NextMove::Forward;
}
	
void Pacman::SetDestination(D3DXVECTOR3 Pos)
{
	SetDestination(Pos.x, Pos.y, Pos.z);
}

void Pacman::CalculateMove(vector<D3DXVECTOR3> availableMoves)
{
	D3DXVECTOR3 front, right, back, left;

	front = D3DXVECTOR3(0,0,-1);
	D3DXVec3TransformCoord(&front, &front, &GetRotationMatrix());

	D3DXMATRIX rotate90;
	D3DXMatrixRotationY(&rotate90, PI/2);
	D3DXVec3TransformCoord(&right, &front, &rotate90);
	D3DXVec3TransformCoord(&back, &right, &rotate90);
	D3DXVec3TransformCoord(&left, &back, &rotate90);

	bool moved = false;
	if (gNextMove == NextMove::Forward)
		moved = (TryToMove(front, availableMoves));

	else if (gNextMove == NextMove::Back)
		moved = (TryToMove(back, availableMoves));

	else if (gNextMove == NextMove::Left)
		moved = (TryToMove(left, availableMoves));

	else if (gNextMove == NextMove::Right)
		moved = (TryToMove(right, availableMoves));
		
	if (!moved)
		TryToMove(front, availableMoves);
}

bool Pacman::TryToMove(D3DXVECTOR3 direction, vector<D3DXVECTOR3> availableMoves)
{
	float EPSILON = 0.05f;
	for (int i = 0; i < availableMoves.size(); i++)
	{
		D3DXVECTOR3 playerPos	= GetPosition();

		D3DXVECTOR3 movepos		= availableMoves[i];
		movepos.y = playerPos.y;

		D3DXVECTOR3 dir = movepos - playerPos;

		D3DXVec3Normalize(&dir, &dir);

		float angle = acos(D3DXVec3Dot(&dir, &direction));
		
		if (angle < EPSILON)
		{
			D3DXVECTOR3	destination	=	availableMoves[i];
			destination.y	=	playerPos.y;
			SetDestination(destination);
			return true;
		}
	}
	return false;
}

void Pacman::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	if(gCooldown < 0)
		gCooldown	=	0;
	else
		gCooldown	-=	deltaTime;
}

bool Pacman::InCherryMode()
{
	return (gCooldown > 0);
}