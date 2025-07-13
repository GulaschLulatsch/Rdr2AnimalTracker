#pragma once

#include "EMenuItemType.h"
#include "MenuBase.h"
#include "MenuItemDefault.h"

#include <memory>
#include <string>

class MenuItemMenu : public MenuItemDefault
{
public:
	virtual ~MenuItemMenu() = default;

	virtual void OnDraw(float lineTop, float lineLeft, bool active) const override;

	void OnRight() override;

protected:
	MenuItemMenu(std::string const& caption, std::unique_ptr<MenuBase> menu);
private:
	std::unique_ptr<MenuBase> m_menu;
};
