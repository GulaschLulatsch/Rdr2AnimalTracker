#include "MenuItemBase.h"

#include "DrawUtils.h"
#include "MenuBase.h"
#include "MenuController.h"

#include "RDR2ScriptHook/natives.h"

void MenuItemBase::OnDraw(float lineTop, float lineLeft, bool active) const
{
	// text
	ColorRgba color = active ? m_colorTextActive : m_colorText;
	UI::SET_TEXT_SCALE(0.0, m_itemHeight * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, color.a);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	DrawUtils::DrawText(lineLeft + m_textLeftMargin, lineTop + m_itemHeight / 4.5f, GetCaption());
	// rect
	DrawUtils::DrawRect(lineLeft, lineTop, m_itemWidth, m_itemHeight, active ? m_colorRectActive : m_colorRect);
}

float MenuItemBase::GetItemWidth() const
{
	return m_itemWidth;
}

float MenuItemBase::GetItemHeight() const
{
	return m_itemHeight;
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

void MenuItemBase::SetMenu(MenuBase* menu)
{
	m_menu = menu;
}

MenuBase* MenuItemBase::GetMenu() const
{
	return m_menu;
}

MenuItemBase::MenuItemBase(
	float itemWidth,
	float itemHeight,
	float textLeftMargin,
	ColorRgba colorRect,
	ColorRgba colorText,
	ColorRgba colorRectActive,
	ColorRgba colorTextActive
) :
	m_itemWidth{ itemWidth },
	m_itemHeight{ itemHeight },
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

void MenuItemBase::SetStatusText(std::string const& text, int ms)
{
	if(m_menu){
		MenuController* controller{ m_menu->GetController() };
		if (controller){
			controller->SetStatusText(text, ms);
		}
	}
}