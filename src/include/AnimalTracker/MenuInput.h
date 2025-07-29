#pragma once

struct MenuInputButtonState
{
	bool select, cancel, up, down, left, right;
};

namespace MenuInput
{
	bool MenuSwitchPressed();

	MenuInputButtonState GetButtonState();
}