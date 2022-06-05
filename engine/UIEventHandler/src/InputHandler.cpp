#include "InputHandler.h"

#include "dependencies/glfw-3.3.2.bin.WIN32/include/GLFW/glfw3.h"
#include <chrono>
#include <thread>

InputHandler::InputHandler(GLFWwindow* _window)
{
	lastUpdate = std::chrono::high_resolution_clock::now();
	now = std::chrono::high_resolution_clock::now();
	window = _window;

	resetMouseInfo();

	attach();
	update();
	detach();

	setMousePositionCenter();

	//glfwSetCursorPosCallback(window, cursor_position_callback);
	//glfwSetScrollCallback(window, scroll_callback);
}

void InputHandler::getWindowSize(int* x, int* y)
{
	glfwGetWindowSize(window, x, y);
}

void InputHandler::setWindowSize(int x, int y)
{
	glfwSetWindowSize(window, x, y);
}

void InputHandler::getMousePosition(double* x, double* y)
{
	glfwGetCursorPos(window, x, y);
}

void InputHandler::setMousePosition(double x, double y)
{
	glfwSetCursorPos(window, x, y);
}

void InputHandler::setMousePositionCenter()
{
	int windowWidth, windowHeight;
	getWindowSize(&windowWidth, &windowHeight);
	setMousePosition(windowWidth / 2, windowHeight / 2);
}
void InputHandler::getMouseScroll(int* s)
{

}

void InputHandler::attach()
{
	isAttached = true;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	setMousePositionCenter();
}
void InputHandler::detach()
{
	isAttached = false;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	setMousePositionCenter();
}

void InputHandler::processMouse()
{
	if (isAttached == false)
	{
		return;
	}

	getMousePosition(&mouseDeltaX, &mouseDeltaY);
	setMousePositionCenter();

	glfwPollEvents();
	/*if (event.type == sf::Event::MouseWheelScrolled)
	{
		this->mouseDeltaScroll = event.mouseWheelScroll.delta;
	}
	else
	{
		this->mouseDeltaScroll = 0;
	}*/
}

void InputHandler::processKeys()
{
	if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && (keyS == false))
		keyW = true;
	else keyW = false;
	if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && (keyW == false))
		keyS = true;
	else keyS = false;

	if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && (keyA == false))
		keyD = true;
	else keyD = false;
	if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && (keyD == false))
		keyA = true;
	else keyA = false;

	if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (keyLShift == false))
		keySpace = true;
	else keySpace = false;
	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) && (keySpace == false))
		keyLShift = true;
	else keyLShift = false;
}

void InputHandler::update()
{
	// Update timers.
	now = std::chrono::high_resolution_clock::now();
	dt = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastUpdate).count();
	lastUpdate = std::chrono::high_resolution_clock::now();

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		if (isAttached)
			detach();
		else
			attach();
		afterPushDelay();
	}
	if (isAttached == false)
	{
		return;
	}
	processMouse();
	processKeys();
}

void InputHandler::resetMouseInfo()
{
	prevMouseX = 0;
	prevMouseY = 0;
	mouseDeltaX = 0.0;
	mouseDeltaY = 0.0;
	mouseDeltaScroll = 0.0f;
}

void InputHandler::afterPushDelay()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
