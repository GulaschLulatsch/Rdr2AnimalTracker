#include "scriptmenu.h"

#include "ColorRgba.h"
#include "DrawUtils.h"

#include "RDR2ScriptHook/main.h"
#include "RDR2ScriptHook/natives.h"

#include <sysinfoapi.h>
#include <string>

void MenuItemBase::WaitAndDraw(int ms)
{
	auto time = GetTickCount64() + ms;
	bool waited = false;
	while (GetTickCount64() < time || !waited)
	{
		WAIT(0);
		waited = true;
		if (auto menu = GetMenu())
			menu->OnDraw();
	}
}

void MenuItemBase::SetStatusText(string text, int ms)
{
	MenuController* controller;
	if (m_menu && (controller = m_menu->GetController()))
		controller->SetStatusText(text, ms);
}

void MenuItemBase::OnDraw(float lineTop, float lineLeft, bool active)
{
	// text
	ColorRgba color = active ? m_colorTextActive : m_colorText;
	UI::SET_TEXT_SCALE(0.0, m_lineHeight * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, color.a);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	DrawUtils::DrawText(lineLeft + m_textLeft, lineTop + m_lineHeight / 4.5f, const_cast<char*>(GetCaption().c_str()));
	// rect
	color = m_colorRect.adjustBrightness(active ? 1.25 : 1);
	DrawUtils::DrawRect(lineLeft, lineTop, m_lineWidth, m_lineHeight, color);
}

void MenuItemSwitchable::OnDraw(float lineTop, float lineLeft, bool active)
{
	MenuItemDefault::OnDraw(lineTop, lineLeft, active);
	float lineWidth = GetLineWidth();
	float lineHeight = GetLineHeight();
	ColorRgba color = active ? GetColorTextActive() : GetColorText();
	UI::SET_TEXT_SCALE(0.0, lineHeight * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, static_cast<int>(color.a / 1.1f));
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	DrawUtils::DrawText(lineLeft + lineWidth - lineWidth / 6.35f, lineTop + lineHeight / 4.8f, GetState() ? "[Y]" : "[N]");
}

void MenuItemMenu::OnDraw(float lineTop, float lineLeft, bool active)
{
	MenuItemDefault::OnDraw(lineTop, lineLeft, active);
	float lineWidth = GetLineWidth();
	float lineHeight = GetLineHeight();
	ColorRgba color = active ? GetColorTextActive() : GetColorText();
	UI::SET_TEXT_SCALE(0.0, lineHeight * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, color.a / 2);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	DrawUtils::DrawText(lineLeft + lineWidth - lineWidth / 8, lineTop + lineHeight / 3.5f, "*");
}

void MenuItemMenu::OnSelect()
{
	if (auto parentMenu = GetMenu())
		if (auto controller = parentMenu->GetController())
			controller->PushMenu(m_menu);
}


void MenuItemFlush::OnSelect() {
	selectedAnimalsNames->clear();
	(*currentAnimalsNames) = oldAnimalsNames;
	for (MenuItemBase* item : GetMenu()->GetItems()) {
		item->SetColorRect(MenuItemDefault_colorRect);
	}
}

void MenuItemAnimals::OnSelect() {
	auto it = selectedAnimalsNames->find(hash);
	if (it == selectedAnimalsNames->end()) {
		// not present, add it to map
		selectedAnimalsNames->insert({ hash, GetCaption() });
		SetColorRect(MenuItemDefault_colorRectActive);
	}
	else {
		// if present, remove it
		selectedAnimalsNames->erase(hash);
		SetColorRect(MenuItemDefault_colorRect);
	}
	(*animalsNames) = (*selectedAnimalsNames);
}

void MenuController::DrawStatusText()
{
	if (GetTickCount64() < m_statusTextMaxTicks)
	{
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		DrawUtils::DrawText(0.5, 0.5, const_cast<char*>(m_statusText.c_str()));
	}
}