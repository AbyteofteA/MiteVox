
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

	private:

		GLFWwindow* window = nullptr;

	public:

		// Renderer Settings

		int screenWidth = SCREEN_WIDTH;
		int screenHeight = SCREEN_HEIGHT;
		unsigned char backfaceCulling = 1;

		int primitiveShaderID = -1;

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