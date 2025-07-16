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

	MenuBase(
		std::unique_ptr<MenuItemTitle> itemTitle, 
		float menuTop = MENU_TOP, 
		float menuLeft = MENU_LEFT, 
		float itemWidth = ITEM_WIDTH, 
		float itemHeight = ITEM_HEIGHT
	);

	void AddItem(std::unique_ptr<IMenuItem> item);
	void SetController(MenuController* controller);

	size_t GetActiveItemIndex() const;
	MenuController* GetController() const;
	const std::vector<std::unique_ptr<IMenuItem>>& GetItems() const;

	float GetItemsWidth() const;
	float GetItemsHeight() const;

	void OnDraw() const;
	int OnInput();

	static constexpr const float ITEM_WIDTH{ 0.30f };
	static constexpr const float ITEM_HEIGHT{ 0.05f };
	static constexpr const float MENU_TOP{ 0.05f };
	static constexpr const float MENU_LEFT{ 0.f };

private:
	static constexpr const float ROW_MARGIN{ 6.f / 1080.f }; //10 pixel at 1080p
	static constexpr const size_t ROWS_PER_SCREEN{ 11 };

	std::unique_ptr<MenuItemTitle> m_itemTitle;
	float m_menuTop;
	float m_menuLeft;
	float m_itemWidth;
	float m_itemHeight;
	std::vector<std::unique_ptr<IMenuItem>> m_items{};

	size_t m_activeRowIndex{ 0 };
	size_t m_activeScreenIndex{ 0 };

	MenuController* m_controller{ nullptr };
};
