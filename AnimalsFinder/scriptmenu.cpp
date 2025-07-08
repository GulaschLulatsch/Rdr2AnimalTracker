#include "scriptmenu.h"

#include <algorithm>

void DrawText(float x, float y, char* str)
{
	UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", str), x, y);
}

void DrawRect(float lineLeft, float lineTop, float lineWidth, float lineHeight, int r, int g, int b, int a)
{
	float outline = 0.002f;

	// Center coordinates for the filled rectangle
	float centerX = lineLeft + (lineWidth * 0.5f);
	float centerY = lineTop + (lineHeight * 0.5f);

	// Draw filled rectangle
	GRAPHICS::DRAW_RECT(centerX, centerY, lineWidth, lineHeight, r, g, b, a, 0, 0);

	// Draw outline rectangles
	// Top
	//GRAPHICS::DRAW_RECT(centerX, lineTop - (outline * 0.5f), lineWidth + outline * 2, outline, r, g, b, a, 0, 0);
	// Bottom
	GRAPHICS::DRAW_RECT(centerX, lineTop + lineHeight + (outline * 0.5f), lineWidth + outline * 2, outline, 0, 0, 0, 255, 0, 0);
	// Left
	//GRAPHICS::DRAW_RECT(lineLeft - (outline * 0.5f), centerY, outline, lineHeight, 0, 0, 0, 255, 0, 0);
	// Right
	GRAPHICS::DRAW_RECT(lineLeft + lineWidth + (outline * 0.5f), centerY, outline, lineHeight, 0, 0, 0, 255, 0, 0);
}

void MenuItemBase::WaitAndDraw(int ms)
{
	auto time = GetTickCount64() + ms;
	bool waited = false;
	while (GetTickCount64() < time || !waited)
	{
		WAIT(0);
		waited = true;
		if (auto menu = GetMenu())
			menu->OnDraw();
	}
}

void MenuItemBase::SetStatusText(string text, int ms)
{
	MenuController* controller;
	if (m_menu && (controller = m_menu->GetController()))
		controller->SetStatusText(text, ms);
}

void MenuItemBase::OnDraw(float lineTop, float lineLeft, bool active)
{
	// text
	ColorRgba color = active ? m_colorTextActive : m_colorText;
	UI::SET_TEXT_SCALE(0.0, m_lineHeight * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, color.a);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	DrawText(lineLeft + m_textLeft, lineTop + m_lineHeight / 4.5f, const_cast<char*>(GetCaption().c_str()));
	// rect
	color = m_colorRect.adjustBrightness(active ? 1.25 : 1);
	DrawRect(lineLeft, lineTop, m_lineWidth, m_lineHeight, color.r, color.g, color.b, color.a);
}

void MenuItemSwitchable::OnDraw(float lineTop, float lineLeft, bool active)
{
	MenuItemDefault::OnDraw(lineTop, lineLeft, active);
	float lineWidth = GetLineWidth();
	float lineHeight = GetLineHeight();
	ColorRgba color = active ? GetColorTextActive() : GetColorText();
	UI::SET_TEXT_SCALE(0.0, lineHeight * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, static_cast<int>(color.a / 1.1f));
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	DrawText(lineLeft + lineWidth - lineWidth / 6.35f, lineTop + lineHeight / 4.8f, GetState() ? "[Y]" : "[N]");
}

void MenuItemMenu::OnDraw(float lineTop, float lineLeft, bool active)
{
	MenuItemDefault::OnDraw(lineTop, lineLeft, active);
	float lineWidth = GetLineWidth();
	float lineHeight = GetLineHeight();
	ColorRgba color = active ? GetColorTextActive() : GetColorText();
	UI::SET_TEXT_SCALE(0.0, lineHeight * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, color.a / 2);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	DrawText(lineLeft + lineWidth - lineWidth / 8, lineTop + lineHeight / 3.5f, "*");
}

void MenuItemMenu::OnSelect()
{
	if (auto parentMenu = GetMenu())
		if (auto controller = parentMenu->GetController())
			controller->PushMenu(m_menu);
}


void MenuItemFlush::OnSelect() {
	this->selectedAnimalsNames->clear();
	(*this->currentAnimalsNames) = this->oldAnimalsNames;
	for (MenuItemBase* item : this->GetMenu()->GetItems()) {
		item->SetColorRect(MenuItemDefault_colorRect);
	}
}

void MenuItemAnimals::OnSelect() {
	auto& selectedMap = *this->selectedAnimalsNames; 
	auto it = selectedMap.find(hash);
	if (it == selectedMap.end()) {
		// not present, add it to map
		selectedMap[hash] = name;
		this->SetColorRect(MenuItemDefault_colorRectActive);
	}
	else {
		// if present, remove it
		selectedMap.erase(hash);
		this->SetColorRect(MenuItemDefault_colorRect);
	}
	(*this->animalsNames) = (*this->selectedAnimalsNames);
}

void MenuBase::OnDraw()
{
	float lineTop = MenuBase_menuTop;
	float lineLeft = MenuBase_menuLeft;
	if (m_itemTitle->GetClass() == eMenuItemClass::ListTitle)
		reinterpret_cast<MenuItemListTitle*>(m_itemTitle)->
		SetCurrentItemInfo(GetActiveItemIndex() + 1, static_cast<int>(m_items.size()));
	m_itemTitle->OnDraw(lineTop, lineLeft, false);
	lineTop += m_itemTitle->GetLineHeight();
	for (int i = 0; i < MenuBase_linesPerScreen; i++)
	{
		int itemIndex = m_activeScreenIndex * MenuBase_linesPerScreen + i;
		if (itemIndex == m_items.size())
			break;
		MenuItemBase* item = m_items[itemIndex];
		item->OnDraw(lineTop, lineLeft, m_activeLineIndex == i);
		lineTop += item->GetLineHeight() - item->GetLineHeight() * MenuBase_lineOverlap;
	}
}

int MenuBase::OnInput()
{
	const int itemCount = static_cast<int>(m_items.size());
	const int itemsLeft = itemCount % MenuBase_linesPerScreen;
	const int screenCount = itemCount / MenuBase_linesPerScreen + (itemsLeft ? 1 : 0);
	const int lineCountLastScreen = itemsLeft ? itemsLeft : MenuBase_linesPerScreen;

	auto buttons = MenuInput::GetButtonState();

	int waitTime = 0;

	if (buttons.a || buttons.b || buttons.up || buttons.down)
	{
		MenuInput::MenuInputBeep();
		waitTime = buttons.b ? 200 : 150;
	}

	if (buttons.a)
	{
		int activeItemIndex = GetActiveItemIndex();
		m_items[activeItemIndex]->OnSelect();
	}
	else
		if (buttons.b)
		{
			if (auto controller = GetController())
				controller->PopMenu();
		}
		else
			if (buttons.up)
			{
				if (m_activeLineIndex-- == 0)
				{
					if (m_activeScreenIndex == 0)
					{
						m_activeScreenIndex = screenCount - 1;
						m_activeLineIndex = lineCountLastScreen - 1;
					}
					else
					{
						m_activeScreenIndex--;
						m_activeLineIndex = MenuBase_linesPerScreen - 1;
					}
				}
			}
			else
				if (buttons.down)
				{
					m_activeLineIndex++;
					if (m_activeLineIndex == ((m_activeScreenIndex == (screenCount - 1)) ? lineCountLastScreen : MenuBase_linesPerScreen))
					{
						if (m_activeScreenIndex == screenCount - 1)
							m_activeScreenIndex = 0;
						else
							m_activeScreenIndex++;
						m_activeLineIndex = 0;
					}
				}

	return waitTime;
}

void MenuController::DrawStatusText()
{
	if (GetTickCount64() < m_statusTextMaxTicks)
	{
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		DrawText(0.5, 0.5, const_cast<char*>(m_statusText.c_str()));
	}
}

ColorRgba ColorRgba::adjustBrightness(float factor) const
{
	auto adjust = [factor](byte c) -> byte {
		return static_cast<byte>((std::max)(0.0f, (std::min)(255.0f, c * factor)));
	};
	return ColorRgba{ adjust(r), adjust(g), adjust(b), a };
}
