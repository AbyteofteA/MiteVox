
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "dependencies/glfw-3.3.2.bin.WIN32/include/GLFW/glfw3.h"
#include <chrono>

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

	InputHandler(GLFWwindow* _window);

	void getWindowSize(int* x, int* y);
	void setWindowSize(int x, int y);
	void getMousePosition(double* x, double* y);
	void setMousePosition(double x, double y);
	void setMousePositionCenter();
	void getMouseScroll(int* s);

	void attach();
	void detach();

	void processMouse();
	void processKeys();
	void update();

private:

	void resetMouseInfo();
	void afterPushDelay();
};

#endif