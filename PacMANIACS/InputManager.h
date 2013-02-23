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


public:
	InputManager(void);

	void	Update();
	bool	IsKeyPressed(char Key);
	bool	IsKeyDown(char Key);
	bool	IsKeyReleased(char Key);
};

#endif