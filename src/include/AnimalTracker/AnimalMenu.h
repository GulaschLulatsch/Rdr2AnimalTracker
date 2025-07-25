#pragma once

#include "MenuBase.h"

#include "IMenuItem.h"
#include "MenuItemAnimal.h"
#include "MenuItemTitle.h"

#include <memory>
#include <vector>

class AnimalMenu : public MenuBase {
public:
	AnimalMenu(std::unique_ptr<MenuItemTitle> title, float itemWidth = ITEM_WIDTH, float itemHeight = ITEM_HEIGHT);

	void AddItem(std::unique_ptr<MenuItemAnimal> animalInfo);

	// Inherited via MenuBase
	IMenuItem* GetItem(size_t index) const override;

	size_t GetItemCount() const override;

private:
	std::vector<std::unique_ptr<MenuItemAnimal>> m_items{};
};