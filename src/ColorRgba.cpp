#include "ColorRgba.h"

#include <utility>

ColorRgba::ColorRgba() :
	r{ 0 },
	g{ 0 },
	b{ 0 },
	a{ 255 }
{}

ColorRgba::ColorRgba(
	unsigned char red, 
	unsigned char green, 
	unsigned char blue, 
	unsigned char alpha
) :
	r{ red }, g{ green }, b{ blue }, a{ alpha }
{}

ColorRgba ColorRgba::adjustBrightness(double factor) const
{
	auto adjust = [factor](unsigned char c) -> unsigned char {
		return static_cast<unsigned char>((std::max)(0.0, (std::min)(255.0, static_cast<double>(c) *factor)));
		};
	return ColorRgba{ adjust(r), adjust(g), adjust(b), a };
}
