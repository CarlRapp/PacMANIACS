#include "InputManager.h"

InputManager::InputManager()
{
	totalKeys			=	255;

	keyStateLastUpdate	=	new bool[totalKeys];
	keyStateThisUpdate	=	new bool[totalKeys];
}


void InputManager::Update()
{
	for(int i = 0; i < totalKeys; ++i)
	{
		keyStateLastUpdate[i]	=	keyStateThisUpdate[i];

		keyStateThisUpdate[i]	=	GetAsyncKeyState(i) ? true : false;
	}
}

bool InputManager::IsKeyDown(char Key)
{
	return keyStateThisUpdate[Key];
}

bool InputManager::IsKeyPressed(char Key)
{
	return (keyStateThisUpdate[Key] && !keyStateLastUpdate[Key]);
}

bool InputManager::IsKeyReleased(char Key)
{
	return (!keyStateThisUpdate[Key] && keyStateLastUpdate[Key]);
}
