#include "MenuItemCategory.h"

#include "CategoryInfo.h"
#include "EMenuItemType.h"
#include "IMenu.h"
#include "MenuItemMenu.h"

#include <functional>
#include <memory>
#include <utility>

MenuItemCategory::MenuItemCategory(CategoryInfo& categoryInfo, std::unique_ptr<IMenu> menu) :
	MenuItemMenu{ categoryInfo.GetName(), std::move(menu) },
	m_categoryInfo{ categoryInfo }
{}

const CategoryInfo& MenuItemCategory::GetCategoryInfo() const
{
	return m_categoryInfo;
}

EMenuItemType MenuItemCategory::GetClass() const
{
	return EMenuItemType::Category;
}

void MenuItemCategory::OnDraw(float lineTop, float lineLeft, bool active) const
{
	MenuItemMenu::OnDraw(lineTop, lineLeft, active);
	DrawQuality(m_categoryInfo, lineTop, lineLeft);
}

void MenuItemCategory::OnSelect()
{
	m_categoryInfo.RotateQuality();
}
