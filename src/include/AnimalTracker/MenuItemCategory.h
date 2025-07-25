#pragma once

#include "MenuItemMenu.h"

#include "CategoryInfo.h"
#include "EMenuItemType.h"
#include "IMenu.h"

#include <memory>

class MenuItemCategory : public MenuItemMenu
{
public:
	MenuItemCategory(
		CategoryInfo& categoryInfo,
		std::unique_ptr<IMenu> menu
	);

	const CategoryInfo& GetCategoryInfo() const;

	EMenuItemType GetClass() const override;
	void OnDraw(float lineTop, float lineLeft, bool active) const override;
	void OnSelect() override;

private:
	CategoryInfo& m_categoryInfo;
};