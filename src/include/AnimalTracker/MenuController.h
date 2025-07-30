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
	
	void Update(std::set<InputAction> const& input);

private:
	std::vector<IMenu*> m_menuStack{};

	std::string	m_statusText{};
	ULONGLONG	m_statusTextMaxTicks{ 0 };

	IMenu* GetActiveMenu();
	void DrawStatusText() const;
	void OnDraw();
	void OnInput(std::set<InputAction> const& input);
};

