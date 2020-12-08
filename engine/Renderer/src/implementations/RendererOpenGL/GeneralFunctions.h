
#ifndef GENERALFUNCTIONS_H
#define GENERALFUNCTIONS_H

#define PRIMITIVE_BUFFER_SIZE 1024

namespace render
{
	int initRenderer(RendererSettings* renderer)
	{
		if (!glfwInit())
		{
			return -1;
		}

		renderer->screenWidth = SCREEN_WIDTH;
		renderer->screenHeight = SCREEN_HEIGHT;
		renderer->debug = 0;
		renderer->backfaceCulling = 1;

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		//glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);
		

		glfwWindowHint(GLFW_SAMPLES, 2);

		renderer->setWindow(
			glfwCreateWindow(mode->width, mode->height, "MiteVox", monitor, NULL));
		if (renderer->getWindow() == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
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

		// Load standard shaders.

		std::string shadersDir;
		if (renderer->debug)
		{
			shadersDir = "../../engine/Renderer/shaders";
		}
		else
		{
			shadersDir = "../../../../../engine/Renderer/shaders";
		}
		loadShaders(renderer, shadersDir, &standardShaders);
		

		renderer->points.resize(PRIMITIVE_BUFFER_SIZE);
		renderer->lines.resize(PRIMITIVE_BUFFER_SIZE * 2);
		renderer->triangles.resize(PRIMITIVE_BUFFER_SIZE * 3);

		return 1;
	}

	void clearBufferXY(float R = 0.f, float G = 0.f, float B = 0.f)
	{
		glClearColor(R, G, B, 1);
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

	void closeRenderer(RendererSettings* renderer)
	{
		glfwWindowShouldClose(renderer->getWindow());
		delete renderer;
	}
}

#endif