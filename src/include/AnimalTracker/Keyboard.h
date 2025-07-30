#pragma once

#include "KeyboardState.h"

#include <windows.h>

namespace Keyboard{
	KeyboardState& GetKeyboardState();

	void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);
}