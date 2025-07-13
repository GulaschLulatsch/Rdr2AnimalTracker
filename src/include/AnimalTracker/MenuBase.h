#pragma once

#include "MenuItemTitle.h"
#include "IMenuItem.h"

#include <memory>
#include <vector>

class MenuController;

class MenuBase
{
public:
	MenuBase(std::unique_ptr<MenuItemTitle> itemTitle);

	void AddItem(std::unique_ptr<IMenuItem> item);
	void SetController(MenuController* controller);

	size_t GetActiveItemIndex() const;
	MenuController* GetController() const;
	const std::vector<std::unique_ptr<IMenuItem>>& GetItems() const;

	void OnDraw() const;
	void OnFrame() const;
	int OnInput();

private:
	static const float MENU_TOP;
	static const float MENU_LEFT;
	static const float ROW_MARGIN;
	static const size_t ROWS_PER_SCREEN;

	std::unique_ptr<MenuItemTitle> m_itemTitle;
	std::vector<std::unique_ptr<IMenuItem>> m_items{};

	size_t m_activeRowIndex{ 0 };
	size_t m_activeScreenIndex{ 0 };

	MenuController* m_controller{ nullptr };
};
