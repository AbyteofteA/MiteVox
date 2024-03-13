#include "RendererSettings.h"

namespace render
{
	RendererSettings::RendererSettings()
	{

	}

	RendererSettings::~RendererSettings()
	{
		points.deallocate();
		lines.deallocate();
		triangles.deallocate();
	}

	void RendererSettings::set(int _screenWidth, int _screenHeight, bool _isFullScreen, unsigned char _backfaceCulling, ColorRGBf _clearColor)
	{
		screenWidth = _screenWidth;
		screenHeight = _screenHeight;
		isFullScreen = _isFullScreen;
		backfaceCulling = _backfaceCulling;
		clearColor = _clearColor;

		points.reserve(1024);
		lines.reserve(1024 * 2);
		triangles.reserve(1024 * 3);
	}
}