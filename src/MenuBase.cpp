#include "MenuBase.h"

#include "ColorRgba.h"
#include "DrawUtils.h"
#include "IMenuItem.h"
#include "MenuController.h"
#include "MenuInput.h"
#include "MenuItemTitle.h"

#include <algorithm>
#include <memory>
#include <utility>


MenuBase::MenuBase(
	std::unique_ptr<MenuItemTitle> itemTitle,
	float itemWidth,
	float itemHeight
):
	m_itemTitle{ std::move(itemTitle) },
	m_itemWidth{ itemWidth },
	m_itemHeight{ itemHeight }
{
	if(m_itemTitle){
		m_itemTitle->SetMenu(this);
	}
}

size_t MenuBase::GetActiveItemIndex() const {
	return (m_activeScreenIndex * ROWS_PER_SCREEN) + m_activeRowIndex;
}

float MenuBase::GetItemsWidth() const
{
	return m_itemWidth;
}

float MenuBase::GetItemsHeight() const
{
	return m_itemHeight;
}

void MenuBase::SetPosition(float distanceFromLeft, float distanceFromTop)
{
	m_distanceFromLeft = distanceFromLeft;
	m_distanceFromTop = distanceFromTop;
}

void MenuBase::OnDraw() const
{
	//Estimate total height to upshift the menu if it would draw over the screen height
	float estimatedTotalHeight{ (std::min)(GetItemCount(), ROWS_PER_SCREEN) * (GetItemsHeight() + ROW_MARGIN) };
	if (m_itemTitle) {
		estimatedTotalHeight += m_itemTitle->GetItemHeight();
	} else {
		estimatedTotalHeight -= ROW_MARGIN;
	}
	//set draw starting coords
	float lineTop = (std::min)(m_distanceFromTop, 1.f - estimatedTotalHeight);
	float lineLeft = m_distanceFromLeft;

	//draw item title only if present, adjust current y position
	if(m_itemTitle){
		m_itemTitle->OnDraw(lineTop, lineLeft, false);
		lineTop += m_itemTitle->GetItemHeight() + ROW_MARGIN;
	}
	for (size_t i = 0; i < ROWS_PER_SCREEN; i++)
	{
		size_t itemIndex = (m_activeScreenIndex * ROWS_PER_SCREEN) + i;
		// for the last page: stop drawing if end of list is reached
		if (itemIndex == GetItemCount()){
			break;
		}
		IMenuItem* item = GetItem(itemIndex);
		// draw in margin box between elements
		if(m_itemTitle || i > 0){ 
			static const ColorRgba background{0, 0, 0, 80};
			DrawUtils::DrawRect(lineLeft, lineTop - ROW_MARGIN, GetItemsWidth(), ROW_MARGIN, background);
		}
		item->OnDraw(lineTop, lineLeft, m_activeRowIndex == i);
		lineTop += item->GetItemHeight() + ROW_MARGIN;
	}
	
}

void MenuBase::OnInput(MenuController* controller)
{
	const size_t itemCount = GetItemCount();
	const size_t itemsLeft = itemCount % ROWS_PER_SCREEN;
	const size_t screenCount = (itemCount / ROWS_PER_SCREEN) + (itemsLeft ? 1 : 0);
	const size_t lineCountLastScreen = itemsLeft ? itemsLeft : ROWS_PER_SCREEN;

	auto buttons = MenuInput::GetButtonState();

	if (buttons.select)
	{
		GetItem(GetActiveItemIndex())->OnSelect();
		return;
	}
	if (buttons.cancel){
		controller->PopMenu();
		return;
	}
	if (buttons.left) {
		controller->PopMenu();
		return;
	}
	if (buttons.right) {
		GetItem(GetActiveItemIndex())->OnRight(controller);
		return;
	}
	if (buttons.up){
		if (m_activeRowIndex == 0){
			if (m_activeScreenIndex == 0){
				m_activeScreenIndex = screenCount - 1;
				m_activeRowIndex = lineCountLastScreen - 1;
				return;
			}
			m_activeScreenIndex -= 1;
			m_activeRowIndex = ROWS_PER_SCREEN - 1;
			return;
		}
		m_activeRowIndex -= 1;
		return;
	}
	if (buttons.down)
	{
		bool isFinalPage{ m_activeScreenIndex == (screenCount - 1) };
		size_t currentPageMaxIndex{ isFinalPage ? lineCountLastScreen - 1 : ROWS_PER_SCREEN -1 };
		if (m_activeRowIndex == currentPageMaxIndex)
		{
			if (isFinalPage) {
				m_activeScreenIndex = 0;
			}
			else{
				m_activeScreenIndex++;
			}
			m_activeRowIndex = 0;
			return;
		}
		m_activeRowIndex += 1;
		return;
	}
}
