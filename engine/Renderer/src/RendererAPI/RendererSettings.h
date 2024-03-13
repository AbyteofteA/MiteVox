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

#include "engine/Renderer/src/RendererAPI/Color.h"
#include "engine/Renderer/src/RendererAPI/Primitives.h"
#include "engine/CodeSafety/src/SafeArray.h"

#define GLEW_STATIC
#include "dependencies/glew-2.1.0/include/GL/glew.h"
#include "dependencies/glfw-3.3.2.bin.WIN32/include/GLFW/glfw3.h"

namespace render
{
	class RendererSettings
	{
	public:

		int screenWidth = SCREEN_WIDTH;
		int screenHeight = SCREEN_HEIGHT;

		size_t pointLightShadowMapSize = 2048;
		size_t directionalLightShadowMapSize = 2048;
		size_t spotLightShadowMapSize = 2048;

		size_t pointLightsPerCall = 16;
		size_t directionalLightsPerCall = 16;
		size_t spotLightsPerCall = 16;

		size_t pointLightsPerFrame = 256;
		size_t directionalLightsPerFrame = 4;
		size_t spotLightsPerFrame = 256;

		bool isFullScreen;
		unsigned char backfaceCulling = 1;

		int primitiveShaderID = -1;

		ColorRGBf clearColor = { 0 };

		safety::SafeArray<Point> points;
		safety::SafeArray<Point> lines;
		safety::SafeArray<Point> triangles;

		unsigned int amountOfDrawCalls = 0;

		RendererSettings();
		~RendererSettings();
		void set(int _screenWidth, int _screenHeight, bool _isFullScreen, unsigned char _backfaceCulling, ColorRGBf _clearColor);
	};
}

#endif