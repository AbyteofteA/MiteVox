
#ifndef RENDERER_SETTINGS_H
#define RENDERER_SETTINGS_H

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 1280
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 720
#endif
#ifndef MAX_SHADERS
#define MAX_SHADERS 128
#endif

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace render
{
	class RendererSettings
	{
	public:

		int screenWidth = SCREEN_WIDTH;
		int screenHeight = SCREEN_HEIGHT;

		bool isFullScreen;
		unsigned char backfaceCulling = 1;

		int primitiveShaderID = -1;

		ColorRGBf clearColor = { 0 };

		mathem::Buffer<Point> points;
		mathem::Buffer<Point> lines;
		mathem::Buffer<Point> triangles;

		unsigned int amountOfDrawCalls = 0;

		RendererSettings(int _screenWidth, int _screenHeight, bool _isFullScreen, unsigned char _backfaceCulling, ColorRGBf _clearColor)
		{
			screenWidth = _screenWidth;
			screenHeight = _screenHeight;
			isFullScreen = _isFullScreen;
			backfaceCulling = _backfaceCulling;
			clearColor = _clearColor;
		}

		RendererSettings(GLFWwindow* _window)
		{
			setWindow(_window);
		}

		~RendererSettings()
		{
			points.wipe();
			lines.wipe();
			triangles.wipe();
		}

		GLFWwindow* getWindow()
		{
			return window;
		}

		void setWindow(GLFWwindow* _window)
		{
			window = _window;
		}

	private:

		GLFWwindow* window = nullptr;
	};
}

#endif