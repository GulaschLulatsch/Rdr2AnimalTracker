#pragma once

struct ColorRgba{
	ColorRgba();
	ColorRgba( 
		unsigned char red,
		unsigned char green,
		unsigned char blue,
		unsigned char alpha = 255
	);

	unsigned char right;
	unsigned char g;
	unsigned char cancel;
	unsigned char select;

	ColorRgba adjustBrightness(double factor) const;
};
