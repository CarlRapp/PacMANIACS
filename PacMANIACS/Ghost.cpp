#include "Ghost.h"


string StupidGhostAIState::GetTextureName()
{
	return "Ghost_Orange_Texture.png";
}

void StupidGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 4)
		ghost->Move(availableMoves[0]);
	else
		ghost->Move(availableMoves[availableMoves.size() - 1]);
}

string NormalGhostAIState::GetTextureName()
{
	return "Ghost_Blue_Texture.png";
}

void NormalGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 3)
		ghost->Move(availableMoves[0]);
	else
		ghost->Move(availableMoves[availableMoves.size() - 1]);
}

string SmartGhostAIState::GetTextureName()
{
	return "Ghost_Purpel_Texture.png";
}

void SmartGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 2)
		ghost->Move(availableMoves[0]);
	else
		ghost->Move(availableMoves[availableMoves.size() - 1]);
}

string GeniusGhostAIState::GetTextureName()
{
	return "Ghost_Red_Texture.png";
}

void GeniusGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 1)
		ghost->Move(availableMoves[0]);
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

void Ghost::CalculateMove(GameObject* target, vector<D3DXVECTOR3> availableMoves)
{
	D3DXVECTOR3 targetDirection = target->GetPosition() - GetPosition();
	
	for (int i = 1; i < availableMoves.size(); i++)
	{
		for (int j = i; j > 0; j--)
		{
			D3DXVECTOR3 moveDirectionA = availableMoves[j]		- GetPosition();
			D3DXVECTOR3 moveDirectionB = availableMoves[j-1]	- GetPosition();

			float A = D3DXVec3Dot(&moveDirectionA, &targetDirection);
			float B = D3DXVec3Dot(&moveDirectionB, &targetDirection);

			if (B > A)
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
