
#include "MiteVox.h"

int main(int argc, char* argv[])
{
	mitevox::Engine* MiteVox = new mitevox::Engine(argc, argv);
	
	while (!glfwWindowShouldClose(MiteVox->settings->getRendererSettings()->getWindow()))
	{
		GLFWwindow* window = MiteVox->settings->getRendererSettings()->getWindow();
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