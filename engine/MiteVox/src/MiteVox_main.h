
#include "MiteVox.h"

int main(int argc, char* argv[])
{
	mitevox::Engine* MiteVox = new mitevox::Engine(argc, argv);
	
	while (!glfwWindowShouldClose(MiteVox->settings->renderer->getWindow()))
	{
		GLFWwindow* window = MiteVox->settings->renderer->getWindow();
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		MiteVox->update();
	}
	delete MiteVox;

	return 0;
}