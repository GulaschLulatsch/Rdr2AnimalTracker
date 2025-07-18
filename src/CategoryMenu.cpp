#include "CategoryMenu.h"

#include "IMenuItem.h"
#include "MenuBase.h"
#include "MenuItemTitle.h"

#include <memory>
#include <utility>

CategoryMenu::CategoryMenu(std::unique_ptr<MenuItemTitle> title, float itemWidth, float itemHeight):
	MenuBase{
		std::move(title),
		itemWidth,
		itemHeight
	}
{}

void CategoryMenu::AddItem(std::unique_ptr<IMenuItem> menuItem)
{
	menuItem->SetMenu(this);
	m_items.emplace_back(std::move(menuItem));
}

IMenuItem* CategoryMenu::GetItem(size_t index) const {
	return m_items.at(index).get();
}

size_t CategoryMenu::GetItemCount() const {
	return m_items.size();
}