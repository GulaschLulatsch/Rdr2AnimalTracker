#include "MenuBase.h"

#include "MenuItemTitle.h"
#include "MenuInput.h"
#include "MenuController.h"

#include <memory>
#include <type_traits>
#include <vector>

const float MenuBase::MENU_TOP{ 0.05f };
const float MenuBase::MENU_LEFT{ 0.f };
const float MenuBase::ROW_MARGIN{ 10.0f / 1080.0f }; //10 pixel at 1080p
const size_t MenuBase::ROWS_PER_SCREEN{ 11 };

MenuBase::MenuBase(std::unique_ptr<MenuItemTitle> itemTitle):
	m_itemTitle{ std::move(itemTitle) }
{}

void MenuBase::AddItem(std::unique_ptr<IMenuItem> item) {
	item->SetMenu(this); 
	m_items.emplace_back(std::move(item));
}

void MenuBase::SetController(MenuController* controller) {
	m_controller = controller; 
}

size_t MenuBase::GetActiveItemIndex() const {
	return (m_activeScreenIndex * ROWS_PER_SCREEN) + m_activeRowIndex;
}

MenuController* MenuBase::GetController() const
{
	return m_controller;
}

std::vector<std::unique_ptr<IMenuItem>> const& MenuBase::GetItems() const
{
	return m_items;
}

void MenuBase::OnDraw() const
{
	float lineTop = MENU_TOP;
	float lineLeft = MENU_LEFT;
	m_itemTitle->OnDraw(lineTop, lineLeft, false);
	lineTop += m_itemTitle->GetItemHeight() + ROW_MARGIN;
	for (size_t i = 0; i < ROWS_PER_SCREEN; i++)
	{
		size_t itemIndex = (m_activeScreenIndex * ROWS_PER_SCREEN) + i;
		if (itemIndex == m_items.size()){
			break;
		}
		auto const& item = m_items.at(itemIndex);
		item->OnDraw(lineTop, lineLeft, m_activeRowIndex == i);
		lineTop += item->GetItemHeight() + ROW_MARGIN;
	}
}


void MenuBase::OnFrame() const{
	for (auto& item : m_items) {
		item->OnFrame();
	}
}

int MenuBase::OnInput()
{
	const size_t itemCount = m_items.size();
	const size_t itemsLeft = itemCount % ROWS_PER_SCREEN;
	const size_t screenCount = (itemCount / ROWS_PER_SCREEN) + (itemsLeft ? 1 : 0);
	const size_t lineCountLastScreen = itemsLeft ? itemsLeft : ROWS_PER_SCREEN;

	auto buttons = MenuInput::GetButtonState();

	int waitTime = 0;

	//wtf does this if bracket do?
	if (buttons.a || buttons.b || buttons.up || buttons.down || buttons.l || buttons.r)
	{
		waitTime = buttons.b ? 200 : 150;
	}

	if (buttons.a)
	{
		m_items.at(GetActiveItemIndex())->OnSelect();
		return waitTime;
	}
	if (buttons.b){
		if (m_controller){
			m_controller->PopMenu();
			return waitTime;
		}
	}
	if (buttons.l) {
		if (m_controller) {
			m_controller->PopMenu();
			return waitTime;
		}
	}
	if (buttons.r) {
		m_items.at(GetActiveItemIndex())->OnRight();
		return waitTime;
	}
	if (buttons.up){
		if (m_activeRowIndex == 0){
			if (m_activeScreenIndex == 0){
				m_activeScreenIndex = screenCount - 1;
				m_activeRowIndex = lineCountLastScreen - 1;
				return waitTime;
			}
			m_activeScreenIndex -= 1;
			m_activeRowIndex = ROWS_PER_SCREEN - 1;
			return waitTime;
		}
		m_activeRowIndex -= 1;
		return waitTime;
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
			return waitTime;
		}
		return waitTime;
	}

	return waitTime;
}
