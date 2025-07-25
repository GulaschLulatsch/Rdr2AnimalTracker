#include "AnimalMenu.h"

#include "IMenuItem.h"
#include "MenuBase.h"
#include "MenuItemAnimal.h"
#include "MenuItemTitle.h"

#include <memory>
#include <utility>

AnimalMenu::AnimalMenu(std::unique_ptr<MenuItemTitle> title, float itemWidth, float itemHeight):
    MenuBase{
        std::move(title),
        itemWidth,
        itemHeight
    }
{}

void AnimalMenu::AddItem(std::unique_ptr<MenuItemAnimal> animalInfo)
{
    animalInfo->SetMenu(this);
    m_items.emplace_back(std::move(animalInfo));
}

IMenuItem* AnimalMenu::GetItem(size_t index) const
{
    return m_items.at(index).get();
}

size_t AnimalMenu::GetItemCount() const
{
    return m_items.size();
}
