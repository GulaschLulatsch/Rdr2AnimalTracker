#pragma once

#include "InputAction.h"

#include <set>

class MenuController;

class IMenu {
public:
	virtual ~IMenu() = default;

	virtual size_t GetActiveItemIndex() const = 0;

	virtual float GetItemsWidth() const = 0;
	virtual float GetItemsHeight() const = 0;

	virtual void SetPosition(float distanceFromLeft, float distanceFromTop) = 0;
	virtual void OnDraw() const = 0;
	virtual void OnInput(std::set<InputAction> const& inputs, MenuController* controller) = 0;
};