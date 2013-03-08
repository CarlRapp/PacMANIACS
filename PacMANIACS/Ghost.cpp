#include "Ghost.h"


string StupidGhostAIState::GetTextureName()
{
	return "Ghost_Orange_Texture.png";
}

void StupidGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 4)
		ghost->SetDestination(availableMoves[3]);
	else
		ghost->SetDestination(availableMoves[availableMoves.size() - 1]);
}

string NormalGhostAIState::GetTextureName()
{
	return "Ghost_Blue_Texture.png";
}

void NormalGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 3)
		ghost->SetDestination(availableMoves[2]);
	else
		ghost->SetDestination(availableMoves[availableMoves.size() - 1]);
}

string SmartGhostAIState::GetTextureName()
{
	return "Ghost_Purpel_Texture.png";
}

void SmartGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 2)
		ghost->SetDestination(availableMoves[1]);
	else
		ghost->SetDestination(availableMoves[availableMoves.size() - 1]);
}

string GeniusGhostAIState::GetTextureName()
{
	return "Ghost_Red_Texture.png";
}

void GeniusGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 1)
		ghost->SetDestination(availableMoves[0]);
}

Ghost::Ghost(void) : GameObject()
{
	gAIState = new NormalGhostAIState();
}


Ghost::~Ghost(void)
{
}

string Ghost::GetName()
{
	return "Ghost";
}

string Ghost::GetTextureName()
{
	return gAIState->GetTextureName();
}

float Ghost::GetHitRadius()
{
	return 3.0f;
}

void Ghost::SetAIState(GhostAIState* AIState)
{
	gAIState = AIState;
}

void Ghost::SetTarget(GameObject* Target)
{
	gTarget	=	Target;
}

void Ghost::CalculateMove(vector<D3DXVECTOR3> availableMoves)
{
	if(availableMoves.size() == 0 || gTarget == NULL)
		return;

	D3DXVECTOR3 targetDirection = gTarget->GetPosition() - GetPosition();
	
	for (int i = 1; i < availableMoves.size(); i++)
	{
		for (int j = i; j > 0; j--)
		{
			D3DXVECTOR3 moveDirectionA = availableMoves[j]		- GetPosition();
			D3DXVECTOR3 moveDirectionB = availableMoves[j-1]	- GetPosition();

			float A = D3DXVec3Dot(&moveDirectionA, &targetDirection);
			float B = D3DXVec3Dot(&moveDirectionB, &targetDirection);

			if (B < A)
			{
				D3DXVECTOR3 temp	= availableMoves[j];
				availableMoves[j]	= availableMoves[j-1];
				availableMoves[j-1]	= temp;
			}
			else 
				break;
		}		
	}

	gAIState->CalculateMove(this, availableMoves);
}

void Ghost::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	float	dX	=	GetPosition().x - gTarget->GetPosition().x;
	float	dZ	=	GetPosition().z - gTarget->GetPosition().z;

	float dAngle	=	atan2(dX, dZ);

	SetRotation(0, dAngle, 0);
}