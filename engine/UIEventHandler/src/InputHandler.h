
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>

class InputHandler
{
public:

	GLFWwindow* window;

	int prevMouseX;
	int prevMouseY;
	double mouseDeltaX;
	double mouseDeltaY;
	float mouseDeltaScroll;

	bool keyW = false, keyS = false;
	bool keyD = false, keyA = false;
	bool keySpace = false, keyLShift = false;

	bool isAttached = false;

	std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
	double dt = 0;
	std::chrono::high_resolution_clock::time_point now;

	inline InputHandler(GLFWwindow* _window);

	inline void getWindowSize(int* x, int* y);
	inline void setWindowSize(int x, int y);
	inline void getMousePosition(double* x, double* y);
	inline void setMousePosition(double x, double y);
	inline void setMousePositionCenter();
	inline void getMouseScroll(int* s);

	inline void attach();
	inline void detach();

	inline void processMouse();
	inline void processKeys();
	inline void update();

private:

	inline void resetMouseInfo();
	inline void afterPushDelay();
};


// IMPLEMENTATION BELOW //


inline InputHandler::InputHandler(GLFWwindow* _window)
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

inline void InputHandler::getWindowSize(int* x, int* y)
{
	glfwGetWindowSize(window, x, y);
}

inline void InputHandler::setWindowSize(int x, int y)
{
	glfwSetWindowSize(window, x, y);
}

inline void InputHandler::getMousePosition(double* x, double* y)
{
	glfwGetCursorPos(window, x, y);
}

inline void InputHandler::setMousePosition(double x, double y)
{
	glfwSetCursorPos(window, x, y);
}

inline void InputHandler::setMousePositionCenter()
{
	int windowWidth, windowHeight;
	getWindowSize(&windowWidth, &windowHeight);
	setMousePosition(windowWidth / 2, windowHeight / 2);
}
inline void InputHandler::getMouseScroll(int* s)
{

}

inline void InputHandler::attach()
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

inline void InputHandler::processMouse()
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

inline void InputHandler::processKeys()
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

inline void InputHandler::update()
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

inline void InputHandler::resetMouseInfo()
{
	prevMouseX = 0;
	prevMouseY = 0;
	mouseDeltaX = 0.0;
	mouseDeltaY = 0.0;
	mouseDeltaScroll = 0.0f;
}

inline void InputHandler::afterPushDelay()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

#endif