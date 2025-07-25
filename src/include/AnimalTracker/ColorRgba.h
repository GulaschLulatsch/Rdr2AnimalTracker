#pragma once

struct ColorRgba{
	ColorRgba();
	ColorRgba( 
		unsigned char red,
		unsigned char green,
		unsigned char blue,
		unsigned char alpha = 255
	);

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	ColorRgba adjustBrightness(double factor) const;
};
