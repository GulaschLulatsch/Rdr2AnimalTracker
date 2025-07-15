#pragma once

#include "ColorRgba.h"
#include "EMenuItemType.h"
#include "MenuItemBase.h"

#include <string>

class MenuItemTitle : public MenuItemBase
{
public:
	MenuItemTitle(std::string const& caption);

	EMenuItemType GetClass() const override; 
	std::string GetCaption() const override;

	void OnSelect() override;
	void OnRight() override;

private:
	static constexpr const float ITEM_WIDTH{ 0.30f };
	static constexpr const float ITEM_HEIGHT{ 0.06f };
	static constexpr const float TEXT_LEFT_MARGIN{ 0.08f };

	static const ColorRgba RECT_COLOR;
	static const ColorRgba TEXT_COLOR;

	std::string m_caption;

};
