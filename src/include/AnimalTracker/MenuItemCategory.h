#pragma once

#include "MenuItemMenu.h"

#include "CategoryInfo.h"
#include "IMenu.h"
#include "EMenuItemType.h"


#include <functional>
#include <memory>

class MenuItemCategory : public MenuItemMenu
{
public:
	MenuItemCategory(
		CategoryInfo& categoryInfo,
		std::unique_ptr<IMenu> menu,
		std::function<void(CategoryInfo const&)> saveFunction
	);

	const CategoryInfo& GetCategoryInfo() const;

	EMenuItemType GetClass() const override;
	void OnDraw(float lineTop, float lineLeft, bool active) const override;
	void OnSelect() override;

private:
	CategoryInfo& m_categoryInfo;
	std::function<void(CategoryInfo const&)> m_saveFunction;
};