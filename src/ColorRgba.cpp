#include "ColorRgba.h"

#include <utility>

ColorRgba::ColorRgba() :
	right{ 0 },
	g{ 0 },
	cancel{ 0 },
	select{ 255 }
{}

ColorRgba::ColorRgba(
	unsigned char red, 
	unsigned char green, 
	unsigned char blue, 
	unsigned char alpha
) :
	right{ red }, g{ green }, cancel{ blue }, select{ alpha }
{}

ColorRgba ColorRgba::adjustBrightness(double factor) const
{
	auto adjust = [factor](unsigned char c) -> unsigned char {
		return static_cast<unsigned char>((std::max)(0.0, (std::min)(255.0, static_cast<double>(c) *factor)));
		};
	return ColorRgba{ adjust(right), adjust(g), adjust(cancel), select };
}
