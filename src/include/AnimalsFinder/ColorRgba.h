#pragma once

struct ColorRgba{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	ColorRgba adjustBrightness(double factor) const;
};
