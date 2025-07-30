#pragma once

#include "ButtonMapping.h"
#include "InputAction.h"

#include <array>
#include <vector>
#include <set>
#include <mutex>

#include <Windows.h>

class KeyboardState {
public:
	KeyboardState();

	void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);

	std::set<InputAction> GetMenuInputOnFrame(std::vector<ButtonMapping> const& buttonMap);

private:
	static constexpr const size_t KEY_COUNT{ 256u };
	struct KeyStates {
		std::array<bool, KEY_COUNT> keyIsDown{};
		std::array<bool, KEY_COUNT> keyPressed{};
		std::array<bool, KEY_COUNT> keyReleased{};
	};

	mutable std::mutex m_mutex{};
	KeyStates m_state{};
};