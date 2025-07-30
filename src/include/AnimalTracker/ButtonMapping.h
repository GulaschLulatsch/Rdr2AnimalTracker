#pragma once

#include "InputAction.h"

struct ButtonMapping {
	InputAction action;
	unsigned int keyCode;
	bool withCtrl{ false };
};