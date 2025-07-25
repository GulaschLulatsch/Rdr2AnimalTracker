#pragma once

#include "ColorRgba.h"
#include "EMenuItemType.h"
#include "MenuItemBase.h"

#include <string>

class MenuController;

class MenuItemTitle : public MenuItemBase
{
public:
	MenuItemTitle(std::string const& caption);

	EMenuItemType GetClass() const override; 
	std::string const& GetCaption() const override;

	void OnSelect() override;
	void OnRight(MenuController* controller) override;

private:
	static constexpr const float TEXT_LEFT_MARGIN{ 0.08f };

	static const ColorRgba RECT_COLOR;
	static const ColorRgba TEXT_COLOR;

	std::string m_caption;

};
