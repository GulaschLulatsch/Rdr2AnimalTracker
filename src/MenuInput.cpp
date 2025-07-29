#include "MenuInput.h"

#include "Keyboard.h"

#include <Windows.h>

bool MenuInput::MenuSwitchPressed()
{
	return Keyboard::WasKeyReleased(VK_F8);
}

MenuInputButtonState MenuInput::GetButtonState()
{
	using namespace Keyboard;
	return {
		WasKeyPressed(VK_NUMPAD5) || IsKeyDownLong(VK_NUMPAD5),
		WasKeyPressed(VK_NUMPAD0) || IsKeyDownLong(VK_NUMPAD0) || WasKeyReleased(VK_F8),
		WasKeyPressed(VK_NUMPAD8) || IsKeyDownLong(VK_NUMPAD8),
		WasKeyPressed(VK_NUMPAD2) || IsKeyDownLong(VK_NUMPAD2),
		WasKeyPressed(VK_NUMPAD4) || IsKeyDownLong(VK_NUMPAD4),
		WasKeyPressed(VK_NUMPAD6) || IsKeyDownLong(VK_NUMPAD6),
	};
}