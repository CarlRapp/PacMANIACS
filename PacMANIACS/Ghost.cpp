#include "Ghost.h"


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
