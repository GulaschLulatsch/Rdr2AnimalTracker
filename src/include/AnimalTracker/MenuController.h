#pragma once

#include "MenuBase.h"

#include "windows.h"
#include "sysinfoapi.h"

#include <string>
#include <vector>

class MenuController
{
public:
	MenuController() = default;
	bool HasActiveMenu() { return m_menuStack.size() > 0; }
	void PushMenu(MenuBase* menu) { if (IsMenuRegistered(menu)) m_menuStack.push_back(menu); }
	void PopMenu() { if (m_menuStack.size()) m_menuStack.pop_back(); }
	void SetStatusText(std::string text, int ms) { m_statusText = text, m_statusTextMaxTicks = GetTickCount64() + ms; }
	bool IsMenuRegistered(MenuBase* menu)
	{
		for (size_t i = 0; i < m_menuList.size(); i++)
			if (m_menuList[i] == menu)
				return true;
		return false;
	}
	void RegisterMenu(MenuBase* menu)
	{
		if (!IsMenuRegistered(menu))
		{
			menu->SetController(this);
			m_menuList.push_back(menu);
		}
	}
	void Update()
	{
		OnDraw();
		OnInput();
		OnFrame();
	}

private:
	std::vector<MenuBase*> m_menuList{};
	std::vector<MenuBase*> m_menuStack{};

	ULONGLONG	m_inputTurnOnTime{ 0 };

	std::string	m_statusText{};
	ULONGLONG	m_statusTextMaxTicks{ 0 };

	void InputWait(int ms) { m_inputTurnOnTime = GetTickCount64() + ms; }
	bool InputIsOnWait() { return m_inputTurnOnTime > GetTickCount64(); }
	MenuBase* GetActiveMenu() { return m_menuStack.size() ? m_menuStack[m_menuStack.size() - 1] : NULL; }
	void DrawStatusText();
	void OnDraw()
	{
		if (auto menu = GetActiveMenu())
			menu->OnDraw();
		DrawStatusText();
	}
	void OnInput()
	{
		if (InputIsOnWait())
			return;
		if (auto menu = GetActiveMenu())
			if (int waitTime = menu->OnInput())
				InputWait(waitTime);
	}
	void OnFrame()
	{
		for (auto i = 0; i < m_menuList.size(); i++)
			m_menuList[i]->OnFrame();
	}
};

