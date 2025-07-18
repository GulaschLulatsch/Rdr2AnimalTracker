#pragma once

#include "MenuBase.h"

#include "IMenuItem.h"
#include "MenuItemTitle.h"

#include <memory>
#include <vector>

class CategoryMenu : public MenuBase {
public:
	CategoryMenu(std::unique_ptr<MenuItemTitle> title, float itemWidth = ITEM_WIDTH, float itemHeight = ITEM_HEIGHT);

	void AddItem(std::unique_ptr<IMenuItem> menuItem);

	IMenuItem* GetItem(size_t index) const override;

	size_t GetItemCount() const override;

private:
	std::vector<std::unique_ptr<IMenuItem>> m_items{};
};