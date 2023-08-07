#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "engine/CodeSafety/src/SafeArray.h"

#include "dependencies/glfw-3.3.2.bin.WIN32/include/GLFW/glfw3.h"
#include <chrono>
#include <mutex>

enum struct KeyAction
{
	RELEASE = GLFW_RELEASE,
	PRESS = GLFW_PRESS,
	REPEAT = GLFW_REPEAT
};

struct KeyEvent
{
	int key;
	int scancode;
	KeyAction action;
	int mods;
};

class InputHandler
{
public:

	GLFWwindow* window;

	double mouseDeltaX;
	double mouseDeltaY;
	float mouseDeltaScroll;

	safety::SafeArray<KeyEvent> pressedKeys;
	safety::SafeArray<KeyEvent> keySequence;
	safety::SafeArray<KeyEvent> keyEventQueue;

	bool isMouseLocked = false;

	std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
	double dt = 0;
	std::chrono::high_resolution_clock::time_point now;

	InputHandler(InputHandler& other) = delete;
	void operator=(const InputHandler&) = delete;

	static void init(GLFWwindow* _window);
	static InputHandler* getInstance();

	void getWindowSize(int* x, int* y);
	void setWindowSize(int x, int y);
	void getMousePosition(double* x, double* y);
	void setMousePosition(double x, double y);
	void setMousePositionCenter();

	void lockMouse();
	void unlockMouse();

	void processMouse();
	void update();

	bool isKeyPressed(int key);
	void appendKeyEvent(KeyEvent keyEvent);

private:

	static InputHandler* instance;
	static std::mutex mutex;

	InputHandler(GLFWwindow* _window);
	~InputHandler();

	void resetMouseInfo();
	void afterPushDelay();
};

#endif