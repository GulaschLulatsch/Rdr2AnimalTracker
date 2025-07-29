#include "Keyboard.h"
#include "KeyboardState.h"

#include <atomic>
#include <mutex>
#include <cstdint>
#include <sysinfoapi.h>
#include <Windows.h>

const int KEYS_SIZE = 255;
std::mutex keyStateMutex{};

struct KeyState{
	std::atomic<bool> isDown{ false };
	std::atomic<bool> wasPressedOnce{ false };
	std::atomic<bool> wasReleasedOnce{ false };
	std::atomic<bool> longPressReported{ false };
	std::atomic<uint64_t> pressTimestampMs{ 0 };
	uint64_t lastLongPressReported{ 0 };
} keyStates[KEYS_SIZE];


void Keyboard::OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	(void)isWithAlt, isExtended, scanCode, repeats;
	if (key >= KEYS_SIZE){
		return;
	}
	KeyState& state = keyStates[key];
	if (!isUpNow && !wasDownBefore){
		state.isDown.store(true, std::memory_order_release);
		state.wasPressedOnce.store(true, std::memory_order_release);
		state.pressTimestampMs.store(GetTickCount64(), std::memory_order_release);
	}else if (isUpNow && wasDownBefore){
		state.isDown.store(false, std::memory_order_release);
		state.wasReleasedOnce.store(true, std::memory_order_release);
	}
}

const int NOW_PERIOD = 100, MAX_DOWN = 5000, MAX_DOWN_LONG = 30000; // ms

bool Keyboard::WasKeyPressed(DWORD key)
{
	return (key < KEYS_SIZE) ? keyStates[key].wasPressedOnce.exchange(false, std::memory_order_acquire) : false;
}

bool Keyboard::WasKeyReleased(DWORD key)
{
	return (key < KEYS_SIZE) ? keyStates[key].wasReleasedOnce.exchange(false, std::memory_order_acquire) : false;
}

bool Keyboard::IsKeyDownLong(DWORD key, uint64_t thresholdMs, uint64_t repeatMs)
{
	if (key >= KEYS_SIZE) {
		return false;
	}
	KeyState& state = keyStates[key];

	if (!state.isDown.load(std::memory_order_acquire)) {
		state.lastLongPressReported = 0;
		return false;
	}
	auto now = GetTickCount64();
	if ((now - state.pressTimestampMs.load(std::memory_order_acquire)) > thresholdMs) {
		if (now - state.lastLongPressReported > repeatMs) {
			state.lastLongPressReported = now;
			return true;
		}
	}
	return false;
}

bool Keyboard::IsKeyDown(DWORD key)
{
	return (key < KEYS_SIZE) ? keyStates[key].isDown.load(std::memory_order_acquire) : false;
}
