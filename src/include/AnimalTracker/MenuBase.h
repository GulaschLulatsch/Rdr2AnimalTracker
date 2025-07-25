#pragma once

#include "IMenu.h"
#include "IMenuItem.h"
#include "MenuItemTitle.h"

#include <memory>

class MenuController;

class MenuBase : public IMenu
{
public:
	virtual ~MenuBase() = default;

	size_t GetActiveItemIndex() const override;

	float GetItemsWidth() const override;
	float GetItemsHeight() const override;

	void SetPosition(float distanceFromLeft, float distanceFromTop) override;
	void OnDraw() const override;
	void OnInput(MenuController* controller) override;

	static constexpr const float ITEM_WIDTH{ 0.30f };
	static constexpr const float ITEM_HEIGHT{ 0.05f };

	static constexpr const float ROW_MARGIN{ 6.f / 1080.f }; //10 pixel at 1080p
protected:
	MenuBase(
		std::unique_ptr<MenuItemTitle> itemTitle,
		float itemWidth = ITEM_WIDTH,
		float itemHeight = ITEM_HEIGHT
	);

	virtual IMenuItem* GetItem(size_t index) const = 0;
	virtual size_t GetItemCount() const = 0;

private:
	static constexpr const size_t ROWS_PER_SCREEN{ 11 };

	std::unique_ptr<MenuItemTitle> m_itemTitle;
	float m_distanceFromLeft{ 0.f };
	float m_distanceFromTop{ 0.f };
	float m_itemWidth;
	float m_itemHeight;

	size_t m_activeRowIndex{ 0 };
	size_t m_activeScreenIndex{ 0 };
};
