#include "Color.h"

namespace render
{
	ColorRGBf ColorRGBf::BLACK()
	{
		return { 0.0f, 0.0f, 0.0f };
	}

	ColorRGBf ColorRGBf::WHITE()
	{
		return { 1.0f, 1.0f, 1.0f };
	}

	ColorRGBf ColorRGBf::RED()
	{
		return { 1.0f, 0.0f, 0.0f };
	}

	ColorRGBf ColorRGBf::GREEN()
	{
		return { 0.0f, 1.0f, 0.0f };
	}

	ColorRGBf ColorRGBf::BLUE()
	{
		return { 0.0f, 0.0f, 1.0f };
	}

	ColorRGBf ColorRGBf::MAGENTA()
	{
		return { 1.0f, 0.0f, 1.0f };
	}

	ColorRGBf ColorRGBf::CYAN()
	{
		return { 0.0f, 1.0f, 1.0f };
	}

	ColorRGBf ColorRGBf::YELLOW()
	{
		return { 1.0f, 1.0f, 0.0f };
	}

	ColorRGBAf ColorRGBAf::NONE()
	{
		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	ColorRGBAf::ColorRGBAf()
	{
		*this = ColorRGBAf(ColorRGBf::WHITE());
	}

	ColorRGBAf::ColorRGBAf(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	ColorRGBAf::ColorRGBAf(ColorRGBf color)
	{
		r = color.r;
		g = color.g;
		b = color.b;
		a = 1.0f;
	}
}