#pragma once

#include "ButtonMapping.h"
#include "InputAction.h"

#include <array>
#include <cstdint>
#include <map>
#include <set>
#include <mutex>

#include <Windows.h>

class KeyboardState {
public:
	KeyboardState();

	void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);

	std::set<InputAction> GetMenuInputOnFrame(std::multimap<InputAction, ButtonMapping> const& buttonMap);

private:
	static constexpr const size_t KEY_COUNT{ 256u };
	struct keyStates {
		std::array<bool, KEY_COUNT> keyIsDown{};
		std::array<bool, KEY_COUNT> keyPressed{};
		std::array<bool, KEY_COUNT> keyReleased{};
		std::array<uint64_t, KEY_COUNT> pressTimestampMs{};
		std::array<uint64_t, KEY_COUNT> lastLongPressReported{};
	};

	mutable std::mutex m_mutex{};
	keyStates m_state{};
};