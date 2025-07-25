#pragma once

#include <windows.h>

#include <cstdint>

namespace Keyboard{
	void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);

	bool IsKeyDown(DWORD key);
	bool IsKeyDownLong(DWORD key, uint64_t thresholdMs = 500, uint64_t repeatMs = 100);
	bool IsKeyJustUp(DWORD key);
	void ClearOnFrame();
}