#pragma once

struct MenuInputButtonState
{
	bool a, b, up, down, l, r;
};

namespace MenuInput
{
	bool MenuSwitchPressed();

	MenuInputButtonState GetButtonState();
}