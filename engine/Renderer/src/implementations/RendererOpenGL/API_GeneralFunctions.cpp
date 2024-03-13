#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

#include <iostream>
#include <cassert>

#define PRIMITIVE_BUFFER_SIZE 1024

namespace render
{
	void initRenderer(ColorRGBf clearColor)
	{
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			// Problem: glewInit failed, something is seriously wrong.
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}

		glDisable(GL_MULTISAMPLE);
		glClearColor((GLclampf)clearColor.r, (GLclampf)clearColor.g, (GLclampf)clearColor.b, (GLclampf)1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		render::setAdditiveBlending();

		getUnitCubeID(); // Allocate the cube with the first call
		getScreenQuadID(); // Allocate with the first call
	}

	std::string getVendorName()
	{
		return std::string((const char*)glGetString(GL_VENDOR));
	}

	std::string getRendererName()
	{
		return std::string((const char*)glGetString(GL_RENDERER));
	}

	std::string getVersion()
	{
		return std::string((const char*)glGetString(GL_VERSION));
	}

	std::string getLanguageVersion()
	{
		return std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	/// <summary>
	/// Source: https://learnopengl.com/In-Practice/Debugging
	/// </summary>
	void printErrors(const char* file, int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}
	}

	void setWireframeRendering(bool isEnabled)
	{
		if (isEnabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void setViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void disableBlending()
	{
		glDisable(GL_BLEND);
	}

	void setAdditiveBlending()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}

	void setAlphaBlending()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}