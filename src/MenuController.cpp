#include "MenuController.h"

#include "DrawUtils.h"

#include <RDR2ScriptHook/natives.h>

void MenuController::DrawStatusText()
{
	if (GetTickCount64() < m_statusTextMaxTicks)
	{
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		DrawUtils::DrawText(0.5, 0.5, const_cast<char*>(m_statusText.c_str()));
	}
}