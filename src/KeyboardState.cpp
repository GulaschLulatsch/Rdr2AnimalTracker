#include "ButtonMapping.h"
#include "InputAction.h"
#include "KeyboardState.h"

#include <Windows.h>

#include <vector>
#include <set>

KeyboardState::KeyboardState()
{
	m_state.keyIsDown.fill(false);
	m_state.keyPressed.fill(false);
	m_state.keyReleased.fill(false);
}
void KeyboardState::OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow) {
	(void)isWithAlt, isExtended, scanCode, repeats;
	if (key >= KEY_COUNT) {
		return;
	}
	if (!isUpNow && !wasDownBefore) {
		std::unique_lock lock{ m_mutex };
		m_state.keyIsDown[key] = true;
		m_state.keyPressed[key] = true;
	}
	else if (isUpNow && wasDownBefore) {
		std::unique_lock lock{ m_mutex };
		m_state.keyIsDown[key] = false;
		m_state.keyReleased[key] = true;
	}
}

std::set<InputAction> KeyboardState::GetMenuInputOnFrame(std::vector<ButtonMapping> const& buttonMap)
{
	KeyStates keyStates{};
	{
		std::unique_lock lock{ m_mutex };
		keyStates = m_state; // make local copy
		m_state.keyPressed.fill(false); // memset these to false so that no key presses linger that where not recalled
		m_state.keyReleased.fill(false);
	}
	std::set<InputAction> returnValue{};
	for (ButtonMapping const& button : buttonMap) {
		if (
			keyStates.keyPressed[button.keyCode] && // base button needs to get pressed
			(
				!button.withCtrl ||  // either control key is not set -> no additional criteria -> true
				(keyStates.keyIsDown[VK_CONTROL] || keyStates.keyPressed[VK_CONTROL]) // or control button is either currently pressed down or was pressed during this frame
			)
		) {
			returnValue.insert(button.action);
		}
	}
	return returnValue;
}
