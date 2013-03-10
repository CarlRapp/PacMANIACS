#include "InputManager.h"

InputManager::InputManager(HWND* Window)
{
	inputWindow			=	Window;

	totalKeys			=	255;
	keyStateLastUpdate	=	new bool[totalKeys];
	keyStateThisUpdate	=	new bool[totalKeys];
}


void InputManager::Update()
{
	if(GetActiveWindow() != *inputWindow)
	{
		mouseMovement	=	D3DXVECTOR2(0, 0);
		return;
	}

	for(int i = 0; i < totalKeys; ++i)
	{
		keyStateLastUpdate[i]	=	keyStateThisUpdate[i];

		keyStateThisUpdate[i]	=	GetAsyncKeyState(i) ? true : false;
	}

	UpdateMouse();
}

void InputManager::UpdateMouse()
{
	LPRECT Rect = new RECT();
	GetWindowRect(*inputWindow, Rect);

	LPPOINT mousePos = new POINT();
	GetCursorPos(mousePos);

	int centerX		=	(Rect->left + Rect->right) * 0.5f;
	int centerY		=	(Rect->top + Rect->bottom) * 0.5f;
	mouseMovement	=	D3DXVECTOR2(mousePos->x - centerX, centerY - mousePos->y);
	SetCursorPos(centerX, centerY);
}

D3DXVECTOR2 InputManager::GetMouseMovement()
{
	return mouseMovement;
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
