#include "MenuInput.h"

#include "Keyboard.h"

#include <Windows.h>

bool MenuInput::MenuSwitchPressed()
{
	return Keyboard::IsKeyJustUp(VK_F8);
}

MenuInputButtonState MenuInput::GetButtonState()
{
	using namespace Keyboard;
	return {
		IsKeyDown(VK_NUMPAD5) || IsKeyDownLong(VK_NUMPAD5),
		IsKeyDown(VK_NUMPAD0) || IsKeyDownLong(VK_NUMPAD0) || IsKeyJustUp(VK_F8),
		IsKeyDown(VK_NUMPAD8) || IsKeyDownLong(VK_NUMPAD8),
		IsKeyDown(VK_NUMPAD2) || IsKeyDownLong(VK_NUMPAD2),
		IsKeyDown(VK_NUMPAD4) || IsKeyDownLong(VK_NUMPAD4),
		IsKeyDown(VK_NUMPAD6) || IsKeyDownLong(VK_NUMPAD6),
	};
}