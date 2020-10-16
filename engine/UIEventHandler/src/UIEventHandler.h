#pragma once

#include "UIEventHandler_Callbacks.h"

class InputHandler
{
public:

	GLFWwindow* window;

	int prevMouseX = 0;
	int prevMouseY = 0;
	double mouseDeltaX = 0;
	double mouseDeltaY = 0;
	float mouseDeltaScroll = 0.f;

	bool keyW = false, keyS = false;
	bool keyD = false, keyA = false;
	bool keySpace = false, keyLShift = false;

	bool isAttached = false;

	std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
	double dt = 0;
	std::chrono::high_resolution_clock::time_point now;

	InputHandler(GLFWwindow* _window)
	{
		lastUpdate = std::chrono::high_resolution_clock::now();
		now = std::chrono::high_resolution_clock::now();
		window = _window;

		//glfwSetCursorPosCallback(window, cursor_position_callback);
		//glfwSetScrollCallback(window, scroll_callback);
	}
	~InputHandler(){}

	
	void getWindowSize(int* x, int* y)
	{
		glfwGetWindowSize(window, x, y);
	}
	void setWindowSize(int x, int y)
	{
		glfwSetWindowSize(window, x, y);
	}
	void getMousePosition(double *x, double *y)
	{
		glfwGetCursorPos(window, x, y);
	}
	void setMousePosition(double x, double y)
	{
		glfwSetCursorPos(window, x, y);
	}
	void setMousePositionCenter()
	{
		int windowWidth, windowHeight;
		getWindowSize(&windowWidth, &windowHeight);
		setMousePosition(windowWidth / 2, windowHeight / 2);
	}
	void getMouseScroll(int* s)
	{
		
	}

	void attach()
	{
		isAttached = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		getMousePosition(&mouseDeltaX, &mouseDeltaY);
		setMousePositionCenter();
	}
	void detach()
	{
		isAttached = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void processMouse()
	{
		if (!isAttached)
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

	void processKeys()
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

	void update()
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
			afterPush();
		}
		if (!isAttached)
		{
			return;
		}
		processMouse();
		processKeys();
	}

	void afterPush()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
};