#include "MenuItemTitle.h"

const float MenuItemTitle::ITEM_WIDTH{ 0.30f };
const float MenuItemTitle::ITEM_HEIGHT{ 0.06f };
const float MenuItemTitle::TEXT_LEFT_MARGIN{ 0.08f };

const ColorRgba MenuItemTitle::RECT_COLOR{ 0, 0, 0, 255 };
const ColorRgba MenuItemTitle::TEXT_COLOR{ 100, 0, 0, 255 };

MenuItemTitle::MenuItemTitle(std::string const& caption) :
	MenuItemBase{ 
		ITEM_WIDTH, 
		ITEM_HEIGHT, 
		TEXT_LEFT_MARGIN, 
		RECT_COLOR, 
		TEXT_COLOR, 
		TEXT_COLOR, 
		RECT_COLOR 
	},
	m_caption{ caption }
{}

EMenuItemType MenuItemTitle::GetClass() const
{
	return EMenuItemType::Title;
}

std::string MenuItemTitle::GetCaption() const
{
	return m_caption;
}

void MenuItemTitle::OnFrame() const
{ /*Do nothing on frame*/ }

void MenuItemTitle::OnSelect()
{ /*Title should be unable to get selected*/ }

void MenuItemTitle::OnRight()
{ /*Title should be unable to get selected*/ }
