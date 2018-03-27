#pragma once

#ifndef COLOR_H
#define COLOR_H

typedef unsigned char byte;

class Color
{
public:
	byte rgba[4];

	inline byte& operator[](const int i)
	{
		return rgba[i];
	}

	Color(byte r, byte g, byte b, byte a = 255)
	{
		rgba[0] = r, rgba[1] = g, rgba[2] = b, rgba[3] = a;
	}
	Color(byte Brightness, byte Alpha = 255)
	{
		rgba[0] = rgba[1] = rgba[2] = Brightness, rgba[3] = Alpha;
	}
	Color() {}

	static Color FromHSB(float Hue, float Saturation, float Brightness, int Alpha = 255);
};
#endif