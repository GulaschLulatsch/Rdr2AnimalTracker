#include "MenuInput.h"

#include "Keyboard.h"

#include <WinUser.h>

bool MenuInput::MenuSwitchPressed()
{
	return Keyboard::IsKeyJustUp(VK_F8);
}

MenuInputButtonState MenuInput::GetButtonState()
{
	using namespace Keyboard;
	return {
		IsKeyDown(VK_NUMPAD5) || (IsKeyDownLong(VK_CONTROL) && IsKeyDown(VK_RETURN)),
		IsKeyDown(VK_NUMPAD0) || (IsKeyDownLong(VK_CONTROL) && IsKeyDown(VK_BACK)),
		IsKeyDown(VK_NUMPAD8) || (IsKeyDownLong(VK_CONTROL) && IsKeyDown(VK_UP)),
		IsKeyDown(VK_NUMPAD2) || (IsKeyDownLong(VK_CONTROL) && IsKeyDown(VK_DOWN)),
		IsKeyDown(VK_NUMPAD4) || (IsKeyDownLong(VK_CONTROL) && IsKeyDown(VK_LEFT)),
		IsKeyDown(VK_NUMPAD6) || (IsKeyDownLong(VK_CONTROL) && IsKeyDown(VK_RIGHT))
	};
}