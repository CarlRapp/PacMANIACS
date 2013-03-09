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

void GhostAIState::SetTargetState(GhostTargetState* State)
{
	gActiveState	=	State;
}

Ghost::Ghost(void) : GameObject()
{
	soundKey = "";
	//gAIState	= new NormalGhostAIState();


	gHuntState	=	new GhostTargetState();
	gFleeState	=	new FleeState();
	((FleeState*)gFleeState)->SetFleeTexture("Flee_Texture.png");
	gCooldown	=	0.0f;
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
	return gAIState->GetActiveTextureName();
}

string GhostAIState::GetActiveTextureName()
{
	return gActiveState->GetTextureName();
}

float Ghost::GetHitRadius()
{
	return RescaleHitRadius(0.858f);
}

int	Ghost::GetValue()
{
	return 1000;
}

void Ghost::SetAIState(GhostAIState* AIState)
{
	gAIState = AIState;
	gHuntState->SetTextureName(gAIState->GetTextureName());
	gFleeState->SetTextureName(gAIState->GetTextureName());

	gAIState->SetTargetState(gHuntState);
}
void GhostAIState::Update(float deltaTime)
{
	gActiveState->Update(deltaTime);
}

void GhostTargetState::Update(float deltaTime)
{

}

void FleeState::Update(float deltaTime)
{
	if(gTimeTicked < 0)
	{
		gDoBlink	=	((rand()%100)>50);
		gTimeTicked	=	((rand()%100)/100);
	}

	gTimeTicked	+=	deltaTime;

	if(gTimeTicked > 0.25f)
	{
		gDoBlink	=	(gDoBlink) ? false : true;
		gTimeTicked	=	0;
	}
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
	gAIState->Update(deltaTime);

	if(gCooldown > 0)
		gCooldown	-=	deltaTime;

	D3DXVECTOR2 direction = D3DXVECTOR2(gTarget->GetPosition().x - GetPosition().x, gTarget->GetPosition().z - GetPosition().z);

	SetLook(direction);
	/*
	float	dX	=	GetPosition().x - gTarget->GetPosition().x;
	float	dZ	=	GetPosition().z - gTarget->GetPosition().z;

	float dAngle	=	atan2(dX, dZ);

	SetRotation(0, dAngle, 0);
	*/

	//	Go back to hunt mode
	//	if cherry time is over....
	if(gCooldown < 0)
	{
		gAIState->SetTargetState(gHuntState);
		gCooldown	=	0.0f;
	}
}

void Ghost::HuntTarget()
{
	gAIState->SetTargetState(gHuntState);
}
void Ghost::FleeTarget(float CherryTime)
{
	gAIState->SetTargetState(gFleeState);
	gCooldown	=	CherryTime;
}

float Ghost::GetSpeed()
{
	return GameObject::GetSpeed();
}


void GhostTargetState::SetTextureName(string TextureName)
{
	gTextureName	=	TextureName;
}

string GhostTargetState::GetTextureName()
{
	string t = gTextureName;
	return gTextureName;
}

string FleeState::GetTextureName()
{
	if(gDoBlink)
		return	gFleeTexture;
	else
		return	gTextureName;
}

void FleeState::SetFleeTexture(string TextureName)
{
	gFleeTexture = TextureName;
}