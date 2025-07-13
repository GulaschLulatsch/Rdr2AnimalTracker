#include "ColorRgba.h"

#include <utility>

ColorRgba ColorRgba::adjustBrightness(double factor) const
{
	auto adjust = [factor](unsigned char c) -> unsigned char {
		return static_cast<unsigned char>((std::max)(0.0, (std::min)(255.0, c * factor)));
		};
	return ColorRgba{ adjust(r), adjust(g), adjust(b), a };
}
