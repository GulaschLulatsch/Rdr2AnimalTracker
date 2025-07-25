#include "MenuItemTitle.h"

#include "ColorRgba.h"
#include "EMenuItemType.h"
#include "MenuItemBase.h"

#include "IMenu.h"
#include <string>

const ColorRgba MenuItemTitle::RECT_COLOR{ 0, 0, 0, 255 };
const ColorRgba MenuItemTitle::TEXT_COLOR{ 100, 0, 0, 255 };

MenuItemTitle::MenuItemTitle(std::string const& caption) :
	MenuItemBase{ 
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

std::string const& MenuItemTitle::GetCaption() const
{
	return m_caption;
}

void MenuItemTitle::OnSelect()
{ /*Title should be unable to get selected*/ }

void MenuItemTitle::OnRight(MenuController*)
{ /*Title should be unable to get selected*/ }
