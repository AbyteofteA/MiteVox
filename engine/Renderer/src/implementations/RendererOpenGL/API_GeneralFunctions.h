
#ifndef GENERALFUNCTIONS_H
#define GENERALFUNCTIONS_H

#define PRIMITIVE_BUFFER_SIZE 1024

namespace render
{
	RendererSettings* initRenderer(int width, int height, bool isFullScreen, bool backfaceCulling, ColorRGBf clearColor)
	{
		RendererSettings* renderer = new render::RendererSettings(width, height, isFullScreen, backfaceCulling, clearColor);

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
		glfwWindowHint(GLFW_SAMPLES, 2);

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


		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			// Problem: glewInit failed, something is seriously wrong.
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}


		glEnable(GL_MULTISAMPLE);
		glClearColor((GLclampf)0.05, (GLclampf)0.05, (GLclampf)0.05, (GLclampf)1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(renderer->getWindow());

		glDepthFunc(GL_LEQUAL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		if (renderer->backfaceCulling)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		renderer->points.resize(PRIMITIVE_BUFFER_SIZE);
		renderer->lines.resize(PRIMITIVE_BUFFER_SIZE * 2);
		renderer->triangles.resize(PRIMITIVE_BUFFER_SIZE * 3);

		return renderer;
	}

	void closeRenderer(RendererSettings* renderer)
	{
		glfwWindowShouldClose(renderer->getWindow());
		glfwTerminate();
	}

	inline std::string getVendorName()
	{
		return std::string((const char*)glGetString(GL_VENDOR));
	}

	inline std::string getRendererName()
	{
		return std::string((const char*)glGetString(GL_RENDERER));
	}

	inline std::string getVersion()
	{
		return std::string((const char*)glGetString(GL_VERSION));
	}

	inline std::string getLanguageVersion()
	{
		return std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	/// <summary>
	/// Source: https://learnopengl.com/In-Practice/Debugging
	/// </summary>
	inline void printErrors(const char* file, int line)
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

	void clearBufferXY(ColorRGBf color = { 0 })
	{
		glClearColor(color.r, color.g, color.b, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void clearBufferZ()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void display(RendererSettings* renderer)
	{
		glfwSwapInterval(1);
		glfwSwapBuffers(renderer->getWindow());
	}
}

#endif