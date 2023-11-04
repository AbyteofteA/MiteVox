#include "RendererSettings.h"

namespace render
{
	RendererSettings::RendererSettings(int _screenWidth, int _screenHeight, bool _isFullScreen, unsigned char _backfaceCulling, ColorRGBf _clearColor)
	{
		screenWidth = _screenWidth;
		screenHeight = _screenHeight;
		isFullScreen = _isFullScreen;
		backfaceCulling = _backfaceCulling;
		clearColor = _clearColor;
	}

	RendererSettings::RendererSettings(GLFWwindow* _window)
	{
		setWindow(_window);
	}

	RendererSettings::~RendererSettings()
	{
		points.deallocate();
		lines.deallocate();
		triangles.deallocate();
	}

	GLFWwindow* RendererSettings::getWindow()
	{
		return window;
	}

	void RendererSettings::setWindow(GLFWwindow* _window)
	{
		window = _window;
	}
}