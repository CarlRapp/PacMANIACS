#ifndef INPUT_H
#define INPUT_H	

#include "stdafx.h"
#include <string>

class InputManager
{
private:
	int		totalKeys;
	bool*	keyStateLastUpdate;
	bool*	keyStateThisUpdate;
	HWND*	inputWindow;

	D3DXVECTOR2	mouseMovement;

	void	UpdateMouse();

public:
	InputManager(HWND* Window);

	void	Update();
	bool	IsKeyPressed(char Key);
	bool	IsKeyDown(char Key);
	bool	IsKeyReleased(char Key);

	D3DXVECTOR2	GetMouseMovement();
};

#endif