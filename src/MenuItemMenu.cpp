#include "MenuItemMenu.h"

#include "ColorRgba.h"
#include "MenuBase.h"
#include "MenuItemDefault.h"
#include "DrawUtils.h"
#include "MenuController.h"

#include "RDR2ScriptHook/natives.h"

#include <memory>
#include <string>
#include <type_traits>

MenuItemMenu::MenuItemMenu(std::string const& caption, std::unique_ptr<MenuBase> menu) :
	MenuItemDefault{ caption },
	m_menu{ std::move(menu) }
{}


void MenuItemMenu::OnRight()
{
	MenuBase* parentMenu = GetMenu();
	if (parentMenu) {
		MenuController* controller = parentMenu->GetController();
		if (controller) {
			controller->PushMenu(m_menu.get());
		}
	}
}


void MenuItemMenu::OnDraw(float lineTop, float lineLeft, bool active) const
{
	MenuItemBase::OnDraw(lineTop, lineLeft, active);

	ColorRgba textColor = active ? GetActiveTextColor() : GetTextColor();
	float width{ GetItemWidth() };
	float height{ GetItemHeight()};

	UI::SET_TEXT_SCALE(0.0, height * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(textColor.r, textColor.g, textColor.b, textColor.a / 2);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	DrawUtils::DrawText(lineLeft + width - width / 8, lineTop + height / 3.5f, ">");
}