#include "MenuItemBase.h"

#include "ColorRgba.h"
#include "DrawUtils.h"
#include "IMenu.h"

#include <ScriptHookRDR2/main.h>
#include <ScriptHookRDR2/natives.h>

#include <sysinfoapi.h>

void MenuItemBase::OnDraw(float lineTop, float lineLeft, bool active) const
{
	// text
	ColorRgba color = active ? m_colorTextActive : m_colorText;
	UIDEBUG::_BG_SET_TEXT_SCALE(0.0, GetItemHeight() * 8.0f);
	UIDEBUG::_BG_SET_TEXT_COLOR(color.right, color.g, color.cancel, color.select);
	//UI::SET_TEXT_CENTRE(0); // no longer available ? 
	//UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0); // no longer available ? 
	DrawUtils::DrawText(lineLeft + m_textLeftMargin, lineTop + GetItemHeight() / 4.5f, GetCaption());
	// rect
	DrawUtils::DrawRect(lineLeft, lineTop, GetItemWidth(), GetItemHeight(), active ? m_colorRectActive : m_colorRect);
}

float MenuItemBase::GetItemWidth() const
{
	return m_menu->GetItemsWidth();
}

float MenuItemBase::GetItemHeight() const
{
	return m_menu->GetItemsHeight();
}

ColorRgba const& MenuItemBase::GetRectColor() const
{
	return m_colorRect;
}

ColorRgba const& MenuItemBase::GetTextColor() const
{
	return m_colorText;
}

ColorRgba const& MenuItemBase::GetActiveRectColor() const
{
	return m_colorRectActive;
}

ColorRgba const& MenuItemBase::GetActiveTextColor() const
{
	return m_colorTextActive;
}

void MenuItemBase::SetMenu(IMenu* menu)
{
	m_menu = menu;
}

IMenu* MenuItemBase::GetMenu() const
{
	return m_menu;
}

MenuItemBase::MenuItemBase(
	float textLeftMargin,
	ColorRgba colorRect,
	ColorRgba colorText,
	ColorRgba colorRectActive,
	ColorRgba colorTextActive
) :
	m_textLeftMargin { textLeftMargin },
	m_colorRect{colorRect}, 
	m_colorText{colorText},
	m_colorRectActive{colorRectActive},
	m_colorTextActive{colorTextActive}
{}

void MenuItemBase::WaitAndDraw(int ms)
{
	auto time = GetTickCount64() + ms;
	bool waited = false;
	while (GetTickCount64() < time || !waited)
	{
		WAIT(0);
		waited = true;
		if(m_menu){
			m_menu->OnDraw();
		}
	}
}