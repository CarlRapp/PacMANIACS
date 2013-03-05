#include "GameObjectState.h"

bool AliveGameObjectState::IsAlive()
{
	return true;
}

bool DeadGameObjectState::IsAlive()
{
	return false;
}

bool IdleGameObjectState::IsAlive()
{
	return true;
}