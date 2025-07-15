#pragma once

#include "IMenuItem.h"
#include "MenuItemTitle.h"

#include "MenuController.h"
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
	int OnInput();

private:
	static constexpr const float MENU_TOP{ 0.05f };
	static constexpr const float MENU_LEFT{ 0.f };
	static constexpr const float ROW_MARGIN{ 6.f / 1080.f }; //10 pixel at 1080p
	static constexpr const size_t ROWS_PER_SCREEN{ 11 };

	std::unique_ptr<MenuItemTitle> m_itemTitle;
	std::vector<std::unique_ptr<IMenuItem>> m_items{};

	size_t m_activeRowIndex{ 0 };
	size_t m_activeScreenIndex{ 0 };

	MenuController* m_controller{ nullptr };
};
