#pragma once

class MenuController;

class IMenu {
public:
	virtual ~IMenu() = default;

	virtual size_t GetActiveItemIndex() const = 0;

	virtual float GetItemsWidth() const = 0;
	virtual float GetItemsHeight() const = 0;

	virtual void SetPosition(float distanceFromLeft, float distanceFromTop) = 0;
	virtual void OnDraw() const = 0;
	virtual int OnInput(MenuController* controller) = 0;
};