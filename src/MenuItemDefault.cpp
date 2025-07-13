#include "MenuItemDefault.h"

const float MenuItemDefault::ITEM_WIDTH{ 0.30f };
const float MenuItemDefault::ITEM_HEIGHT{ 0.05f };
const float MenuItemDefault::TEXT_LEFT_MARGIN{ 0.01f };

const ColorRgba MenuItemDefault::RECT_COLOR{ 181, 21, 2, 135 };
const ColorRgba MenuItemDefault::TEXT_COLOR{ 255, 255, 255, 255 };
const ColorRgba MenuItemDefault::RECT_COLOR_ACTIVE{ 235, 2, 2, 185 };
const ColorRgba MenuItemDefault::TEXT_COLOR_ACTIVE{ 255, 255, 255, 255 };

MenuItemDefault::MenuItemDefault(std::string const& caption) :
	MenuItemBase(
		ITEM_WIDTH,
		ITEM_HEIGHT,
		TEXT_LEFT_MARGIN,
		RECT_COLOR,
		TEXT_COLOR,
		RECT_COLOR_ACTIVE,
		TEXT_COLOR_ACTIVE
	),
	m_caption(caption) 
{}

std::string MenuItemDefault::GetCaption() const {
	return m_caption;
}