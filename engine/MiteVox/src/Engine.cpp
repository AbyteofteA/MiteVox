
#include "Engine.h"

#include "EngineSettings.h"
#include "Playground.h"
#include "engine/Math/src/LinearAlgebra/SquareMatrix.h"

#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	Engine::Engine(int argc, char* argv[])
	{
		mathem::SquareMatrix<float, 4> mat1;
		mathem::SquareMatrix<float, 4> mat2;
		mathem::SquareMatrix<float, 4> mat3 = mat1 + mat2;

		std::string executionDir = fs::path(argv[0]).parent_path().string();
		settings = new EngineSettings(executionDir);

		playground = new Playground();

		onCreate();
	}

	Engine::~Engine()
	{
		onDestroy();
		
		if (playground)
		{
			delete playground;
		}
		
		delete settings;
	}

	void Engine::run()
	{
		// TODO: Hide implementation.
		while (!glfwWindowShouldClose(settings->getRendererSettings()->getWindow()))
		{
			GLFWwindow* window = settings->getRendererSettings()->getWindow();
			glfwPollEvents();
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, true);
			}
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			glViewport(0, 0, width, height);

			onUpdate();

			settings->getInputHandler()->update();
			playground->update();
		}
	}
}
