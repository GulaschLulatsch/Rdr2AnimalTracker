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

	void OnFrame() const override;
	void OnSelect() override;
	void OnRight() override;

private:
	static const float ITEM_WIDTH;
	static const float ITEM_HEIGHT;
	static const float TEXT_LEFT_MARGIN; 

	static const ColorRgba RECT_COLOR;
	static const ColorRgba TEXT_COLOR;

	std::string m_caption;

};
