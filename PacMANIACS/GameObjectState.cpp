#include "GameObjectState.h"

bool AliveGameObjectState::IsAlive()
{
	return true;
}
string AliveGameObjectState::GetName()
{
	return "Alive";
}

bool DeadGameObjectState::IsAlive()
{
	return false;
}
string DeadGameObjectState::GetName()
{
	return "Dead";
}

bool IdleGameObjectState::IsAlive()
{
	return true;
}
string IdleGameObjectState::GetName()
{
	return "Idle";
}