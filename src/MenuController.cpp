#include "MenuController.h"

#include "IMenu.h"
#include "DrawUtils.h"

#include <RDR2ScriptHook/natives.h>

#include <string>
#include <sysinfoapi.h>

bool MenuController::HasActiveMenu() const
{
	 return !m_menuStack.empty();
}

void MenuController::PushMenu(IMenu* menu)
{
	m_menuStack.push_back(menu);
}

void MenuController::PopMenu()
{
	if (!m_menuStack.empty()) {
		m_menuStack.pop_back();
	}
}

void MenuController::SetStatusText(std::string const& text, int ms)
{
	m_statusText = text;
	m_statusTextMaxTicks = GetTickCount64() + ms;
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

bool MenuController::InputIsOnWait() const
{
	return m_inputTurnOnTime > GetTickCount64();
}

IMenu* MenuController::GetActiveMenu()
{ 
	return m_menuStack.empty() ? nullptr: m_menuStack.at(m_menuStack.size() - 1u);
}

void MenuController::DrawStatusText() const
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
	static const float ORIGIN_X{ 0.f };
	static const float ORIGIN_Y{ 0.05f };
	if (!m_menuStack.empty()) {
		m_menuStack.front()->SetPosition(ORIGIN_X, ORIGIN_Y);
	}
	for(IMenu* menu : m_menuStack){
		menu->OnDraw();
	}
	DrawStatusText();
}

void MenuController::OnInput()
{
	if (InputIsOnWait()){
		return;
	}
	if (IMenu* menu = GetActiveMenu()){
		int waitTime = menu->OnInput(this);
		if (waitTime > 0){
			InputWait(waitTime);
		}
	}
}
