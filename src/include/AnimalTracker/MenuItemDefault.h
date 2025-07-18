#pragma once

#include "ColorRgba.h"
#include "MenuItemBase.h"
#include "IInfo.h"

#include <string>

class MenuItemDefault : public MenuItemBase
{
public:
	std::string GetCaption() const override; 
protected:
	MenuItemDefault(std::string const& caption);

	void DrawQuality(const IInfo& info, float lineTop, float lineLeft) const;
private:

	static const float ITEM_WIDTH;
	static const float ITEM_HEIGHT;
	static const float TEXT_LEFT_MARGIN;

	static const ColorRgba RECT_COLOR;
	static const ColorRgba TEXT_COLOR;
	static const ColorRgba RECT_COLOR_ACTIVE;
	static const ColorRgba TEXT_COLOR_ACTIVE;

	std::string	m_caption;
};