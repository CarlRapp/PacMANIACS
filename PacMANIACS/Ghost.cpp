#include "Ghost.h"


string StupidGhostAIState::GetTextureName()
{
	return "Ghost_Orange_Texture.png";
}

void StupidGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 4)
		ghost->MoveTo(availableMoves[0]);
	else
		ghost->MoveTo(availableMoves[availableMoves.size() - 1]);
}

string NormalGhostAIState::GetTextureName()
{
	return "Ghost_Blue_Texture.png";
}

void NormalGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 3)
		ghost->MoveTo(availableMoves[0]);
	else
		ghost->MoveTo(availableMoves[availableMoves.size() - 1]);
}

string SmartGhostAIState::GetTextureName()
{
	return "Ghost_Purpel_Texture.png";
}

void SmartGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 2)
		ghost->MoveTo(availableMoves[0]);
	else
		ghost->MoveTo(availableMoves[availableMoves.size() - 1]);
}

string GeniusGhostAIState::GetTextureName()
{
	return "Ghost_Red_Texture.png";
}

void GeniusGhostAIState::CalculateMove(GameObject* ghost, vector<D3DXVECTOR3> availableMoves)
{
	if (availableMoves.size() >= 1)
		ghost->MoveTo(availableMoves[0]);
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

void Ghost::CalculateMove(GameObject* target)
{

}
