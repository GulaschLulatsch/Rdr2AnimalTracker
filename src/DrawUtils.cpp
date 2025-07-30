#include "ColorRgba.h"
#include "DrawUtils.h"

#include <ScriptHookRDR2/natives.h>

#include <string>

void DrawUtils::DrawText(float x, float y, std::string const& str){
	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING",str.c_str()), x, y);
}

void DrawUtils::DrawRect(float x, float y, float width, float height, ColorRgba color) {
	// Center coordinates for the filled rectangle
	float centerX = x + (width * 0.5f);
	float centerY = y + (height * 0.5f);

	// Draw filled rectangle
	GRAPHICS::DRAW_RECT(centerX, centerY, width, height, color.r, color.g, color.b, color.a, 0, 0);
}