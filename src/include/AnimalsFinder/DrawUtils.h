#pragma once

#include "ColorRgba.h"

#include <string>

namespace DrawUtils {
	void DrawText(float x, float y, std::string const& str);

	void DrawRect(float lineLeft, float lineTop, float lineWidth, float lineHeight, ColorRgba color);
}