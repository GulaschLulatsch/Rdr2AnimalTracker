#pragma once

#include "ColorRgba.h"
#include "EMenuItemType.h"
#include "IMenu.h"

#include <string>

class MenuBase;
class MenuController;

class IMenuItem {
public:
	virtual ~IMenuItem() = default;

	virtual EMenuItemType GetClass() const = 0;

	virtual void OnDraw(float lineTop, float lineLeft, bool active) const = 0;
	virtual	void OnSelect() = 0;
	virtual	void OnRight(MenuController* controller) = 0;

	virtual	std::string GetCaption() const = 0;

	virtual float GetItemWidth() const = 0;
	virtual float GetItemHeight() const = 0;

	virtual ColorRgba const& GetRectColor() const = 0;
	virtual ColorRgba const& GetTextColor() const = 0;

	virtual ColorRgba const& GetActiveRectColor() const = 0;
	virtual ColorRgba const& GetActiveTextColor() const = 0;

	virtual void SetMenu(IMenu* menu) = 0;
	virtual IMenu* GetMenu() const = 0;
};