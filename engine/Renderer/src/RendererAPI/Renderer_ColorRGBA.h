
#ifndef RENDERER_COLORRGBA_H
#define RENDERER_COLORRGBA_H

namespace render
{
	struct ColorRGBA
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};

	struct ColorRGBAf
	{
		float r;
		float g;
		float b;
		float a;
	};

	inline ColorRGBA blendColors(ColorRGBA color1, ColorRGBA color2)
	{
		ColorRGBA result = color2;
		result.a = 255 - (int)(255 - color1.a) * (255 - color2.a);
		if (result.a == 0)
			return result;
		result.r = (unsigned char)((float)color1.r * color1.a / 255 + color2.r * color2.a / 255 * (255 - color1.a)) / result.a;
		result.g = (unsigned char)((float)color1.g * color1.a / 255 + color2.g * color2.a / 255 * (255 - color1.a)) / result.a;
		result.b = (unsigned char)((float)color1.b * color1.a / 255 + color2.b * color2.a / 255 * (255 - color1.a)) / result.a;

		return result;
	}
}

#endif