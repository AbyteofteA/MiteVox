
#ifndef RENDERER_SETTINGS_H
#define RENDERER_SETTINGS_H

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>

namespace render
{
	class RendererSettings
	{

	private:

		GLFWwindow* window = nullptr;

	public:

		// Renderer Settings

		char debug = 1;
		int screenWidth = SCREEN_WIDTH;
		int screenHeight = SCREEN_HEIGHT;
		unsigned char backfaceCulling = 1;

		RendererSettings(){}
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

		mathem::Buffer<Point> points;
		mathem::Buffer<Point> lines;
		mathem::Buffer<Point> triangles;
	};
}

#endif