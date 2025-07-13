#pragma once

#include "MenuBase.h"

#include "windows.h"
#include "sysinfoapi.h"

#include <string>
#include <vector>
#include <stack>

class MenuController
{
public:
	MenuController() = default;

	bool HasActiveMenu() const;
	void PushMenu(MenuBase* menu); 
	void PopMenu();
	void SetStatusText(std::string const& text, int ms);
	bool IsMenuRegistered(MenuBase const* menu) const;
	void RegisterMenu(MenuBase* menu);
	
	void Update();

private:
	std::stack<MenuBase*> m_menuStack{};

	ULONGLONG	m_inputTurnOnTime{ 0 };

	std::string	m_statusText{};
	ULONGLONG	m_statusTextMaxTicks{ 0 };

	void InputWait(int ms);
	bool InputIsOnWait();
	MenuBase* GetActiveMenu();
	void DrawStatusText();
	void OnDraw();
	void OnInput();
};

