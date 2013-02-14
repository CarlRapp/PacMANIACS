#include "InputManager.h"

InputManager::InputManager(void){
}

bool InputManager::IsKeyDown(char key)
{
	if(GetAsyncKeyState(key)){
		return true;
	}
	return false;
}