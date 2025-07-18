#pragma once

#include "IMenu.h"

#include "windows.h"

#include <string>
#include <vector>

class MenuController
{
public:
	MenuController() = default;

	bool HasActiveMenu() const;
	void PushMenu(IMenu* menu);
	void PopMenu();
	void SetStatusText(std::string const& text, int ms);
	
	void Update();

private:
	std::vector<IMenu*> m_menuStack{};

	ULONGLONG	m_inputTurnOnTime{ 0 };

	std::string	m_statusText{};
	ULONGLONG	m_statusTextMaxTicks{ 0 };

	void InputWait(int ms);
	bool InputIsOnWait() const;
	IMenu* GetActiveMenu();
	void DrawStatusText() const;
	void OnDraw();
	void OnInput();
};

