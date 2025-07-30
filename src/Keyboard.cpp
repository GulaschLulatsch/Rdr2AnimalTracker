#include "Keyboard.h"
#include "KeyboardState.h"

#include <Windows.h>

KeyboardState& Keyboard::GetKeyboardState()
{
	static KeyboardState keyboardState{};
	return keyboardState;
}

void Keyboard::OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	GetKeyboardState().OnKeyboardMessage(key, repeats, scanCode, isExtended, isWithAlt, wasDownBefore, isUpNow);
}