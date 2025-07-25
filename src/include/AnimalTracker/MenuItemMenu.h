#pragma once

#include "IMenu.h"

#include "MenuItemDefault.h"

#include <memory>
#include <string>

class MenuController;

class MenuItemMenu : public MenuItemDefault
{
public:
	virtual ~MenuItemMenu() = default;

	virtual void OnDraw(float lineTop, float lineLeft, bool active) const override;

	void OnRight(MenuController* controller) override;

protected:
	MenuItemMenu(std::string const& caption, std::unique_ptr<IMenu> menu);
private:
	std::unique_ptr<IMenu> m_menu;
};
