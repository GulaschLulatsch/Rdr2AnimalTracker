#include "MenuItemMenu.h"

#include "ColorRgba.h"
#include "DrawUtils.h"
#include "MenuBase.h"
#include "MenuController.h"
#include "MenuItemDefault.h"
#include "IMenu.h"
#include "MenuItemBase.h"

#include <ScriptHookRDR2/natives.h>

#include <memory>
#include <string>
#include <utility>

MenuItemMenu::MenuItemMenu(std::string const& caption, std::unique_ptr<IMenu> menu) :
	MenuItemDefault{ caption },
	m_menu{ std::move(menu) }
{}


void MenuItemMenu::OnRight(MenuController* controller)
{
	if (controller) {
		controller->PushMenu(m_menu.get());
	}
}


void MenuItemMenu::OnDraw(float lineTop, float lineLeft, bool active) const
{
	MenuItemBase::OnDraw(lineTop, lineLeft, active);

	ColorRgba color = active ? GetActiveTextColor() : GetTextColor();
	UIDEBUG::_BG_SET_TEXT_SCALE(0.0, GetItemHeight() * 8.0f);
	UIDEBUG::_BG_SET_TEXT_COLOR(color.right, color.g, color.cancel, color.select);
	//UI::SET_TEXT_CENTRE(0); // no longer available ? 
	//UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0); // no longer available ? 
	DrawUtils::DrawText(lineLeft + GetItemWidth() - (32.f/1920.f)/*Margin*/, lineTop + GetItemHeight() / 4.5f, ">");

	m_menu->SetPosition(lineLeft + GetItemWidth() + MenuBase::ROW_MARGIN, lineTop);
}