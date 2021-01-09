
#include "MiteVox.h"

int main(int argc, char* argv[])
{
	mitevox::Engine* MiteVox = new mitevox::Engine(argc, argv);
	
	while (!glfwWindowShouldClose(MiteVox->renderer->getWindow()))
	{
		GLFWwindow* window = MiteVox->renderer->getWindow();
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
	glfwTerminate();

	delete MiteVox;

	return 0;
}