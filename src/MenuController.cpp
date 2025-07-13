#include "MenuController.h"

#include "DrawUtils.h"

#include <RDR2ScriptHook/natives.h>

bool MenuController::HasActiveMenu() const
{
	 return !m_menuStack.empty();
}

void MenuController::PushMenu(MenuBase* menu)
{
	if (IsMenuRegistered(menu)) {
		m_menuStack.push(menu);
	}
}

void MenuController::PopMenu()
{
	if (m_menuStack.size()) {
		m_menuStack.pop();
	}
}

void MenuController::SetStatusText(std::string const& text, int ms)
{
	m_statusText = text;
	m_statusTextMaxTicks = GetTickCount64() + ms;
}

bool MenuController::IsMenuRegistered(MenuBase const* menu) const
{
	return menu->GetController() == this;
}

void MenuController::RegisterMenu(MenuBase* menu)
{
	menu->SetController(this);
}

void MenuController::Update()
{
	OnDraw();
	OnInput();
}

void MenuController::InputWait(int ms)
{
	m_inputTurnOnTime = GetTickCount64() + ms;
}

bool MenuController::InputIsOnWait()
{
	return m_inputTurnOnTime > GetTickCount64();
}

MenuBase* MenuController::GetActiveMenu()
{ 
	return m_menuStack.empty() ? nullptr: m_menuStack.top();
}

void MenuController::DrawStatusText()
{
	if (GetTickCount64() < m_statusTextMaxTicks)
	{
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		DrawUtils::DrawText(0.5, 0.5, m_statusText);
	}
}

void MenuController::OnDraw()
{
	if (MenuBase* menu = GetActiveMenu()){
		menu->OnDraw();
	}
	DrawStatusText();
}

void MenuController::OnInput()
{
	if (InputIsOnWait()){
		return;
	}
	if (MenuBase* menu = GetActiveMenu()){
		int waitTime = menu->OnInput();
		if (waitTime > 0){
			InputWait(waitTime);
		}
	}
}
