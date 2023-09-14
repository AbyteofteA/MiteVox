#ifndef RENDERER_COLOR_H
#define RENDERER_COLOR_H

namespace render
{
	struct ColorRGBf
	{
		float r;
		float g;
		float b;

		static ColorRGBf BLACK();
		static ColorRGBf WHITE();
		static ColorRGBf RED();
		static ColorRGBf GREEN();
		static ColorRGBf BLUE();
		static ColorRGBf MAGENTA();
		static ColorRGBf CYAN();
		static ColorRGBf YELLOW();
	};

	struct ColorRGBAf
	{
		float r;
		float g;
		float b;
		float a;

		ColorRGBAf();
		ColorRGBAf(float r, float g, float b, float a);
		ColorRGBAf(ColorRGBf color);
	};
}

#endif