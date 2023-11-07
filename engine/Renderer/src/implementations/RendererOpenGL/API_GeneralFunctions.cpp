#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

#include <iostream>
#include <cassert>

#define PRIMITIVE_BUFFER_SIZE 1024

namespace render
{
	RendererSettings* initRenderer(int width, int height, bool isFullScreen, bool backfaceCulling, ColorRGBf clearColor)
	{
		RendererSettings* renderer = new RendererSettings(width, height, isFullScreen, backfaceCulling, clearColor);

		if (!glfwInit())
		{
			return nullptr;
		}

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		glfwWindowHint(GLFW_SAMPLES, 4);

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);

		GLFWwindow* window = nullptr;
		if (isFullScreen)
		{
			window = glfwCreateWindow(mode->width, mode->height, "MiteVox", monitor, nullptr);
		}
		else
		{
			window = glfwCreateWindow(mode->width, mode->height, "MiteVox", nullptr, nullptr);
		}
		//glfwHideWindow(window);

		renderer->setWindow(window);

		if (renderer->getWindow() == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return nullptr;
		}
		
		glfwMakeContextCurrent(renderer->getWindow());
		glfwSwapInterval(0);

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			// Problem: glewInit failed, something is seriously wrong.
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}

		glEnable(GL_MULTISAMPLE);
		glClearColor((GLclampf)renderer->clearColor.r, (GLclampf)renderer->clearColor.g, (GLclampf)renderer->clearColor.b, (GLclampf)1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(renderer->getWindow());

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);

		if (renderer->backfaceCulling)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		renderer->points.reserve(PRIMITIVE_BUFFER_SIZE);
		renderer->lines.reserve(PRIMITIVE_BUFFER_SIZE * 2);
		renderer->triangles.reserve(PRIMITIVE_BUFFER_SIZE * 3);

		return renderer;
	}

	void closeRenderer(RendererSettings* renderer)
	{
		glfwWindowShouldClose(renderer->getWindow());
		glfwTerminate();
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

	void activateDefaultFramebuffer(int width, int height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)0);
		glViewport(0, 0, width, height);
	}

	void clearBufferXY(ColorRGBf color = { 0 })
	{
		glClearColor(color.r, color.g, color.b, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void clearBufferZ()
	{
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void display(RendererSettings* renderer)
	{
		glfwSwapBuffers(renderer->getWindow());
	}
}