#include "InputHandler.h"

#include "dependencies/glfw-3.3.2.bin.WIN32/include/GLFW/glfw3.h"
#include <chrono>
#include <thread>
#include <string>

InputHandler* InputHandler::instance{ nullptr };
std::mutex InputHandler::mutex;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputHandler* inputHandler = InputHandler::getInstance();
	KeyEvent newKeyEvent = { key, scancode, (KeyAction)action, mods };
	inputHandler->appendKeyEvent(newKeyEvent);
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputHandler* inputHandler = InputHandler::getInstance();
	inputHandler->mouseDeltaScroll = yoffset;
}

void InputHandler::init(GLFWwindow* _window)
{
	std::lock_guard<std::mutex> lock(mutex);

	if (instance == nullptr)
	{
		instance = new InputHandler(_window);
		instance->pressedKeys.reserve(16);
		instance->keyEventQueue.reserve(1024);

		//glfwSetCursorPosCallback(instance->window, cursor_position_callback);
		glfwSetScrollCallback(instance->window, mouseScrollCallback);
		glfwSetKeyCallback(instance->window, keyCallback);
	}
}

InputHandler* InputHandler::getInstance()
{
	return instance;
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

void InputHandler::lockMouse()
{
	isMouseLocked = true;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	setMousePositionCenter();
}

void InputHandler::unlockMouse()
{
	isMouseLocked = false;
	//mouseDeltaX = 0.0f;
	//mouseDeltaY = 0.0f;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	setMousePositionCenter();
}

void InputHandler::processMouse()
{
	if (isMouseLocked == false)
	{
		return;
	}

	int windowWidth, windowHeight;
	getWindowSize(&windowWidth, &windowHeight); // TODO: store windowWidth, windowHeight

	double currentMousePositionX = 0.0, currentMousePositionY = 0.0f;
	getMousePosition(&currentMousePositionX, &currentMousePositionY);
	mouseDeltaX = windowWidth / 2 - currentMousePositionX;
	mouseDeltaY = windowHeight / 2 - currentMousePositionY;

	setMousePositionCenter();

	glfwPollEvents();
}

void InputHandler::update()
{
	// Update timers.
	now = std::chrono::high_resolution_clock::now();
	dt = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastUpdate).count();
	lastUpdate = std::chrono::high_resolution_clock::now();

	if (isKeyPressed(GLFW_KEY_TAB))
	{
		if (isMouseLocked)
			unlockMouse();
		else
			lockMouse();
		afterPushDelay();
	}
	if (isMouseLocked == false)
	{
		return;
	}
	processMouse();
}

bool InputHandler::isKeyPressed(int key)
{
	size_t pressedKeysCount = pressedKeys.getElementsCount();
	for (size_t i = 0; i < pressedKeysCount; ++i)
	{
		KeyEvent pressedKey = pressedKeys.getElement(i);
		if (pressedKey.key == key)
		{
			return true;
		}
	}
	return false;
}

void InputHandler::appendKeyEvent(KeyEvent keyEvent)
{
	instance->keyEventQueue.appendElement(keyEvent);

	size_t pressedKeysCount = pressedKeys.getElementsCount();
	for (size_t i = 0; i < pressedKeysCount; ++i)
	{
		KeyEvent pressedKey = pressedKeys.getElement(i);
		if (keyEvent.key == pressedKey.key)
		{
			if (keyEvent.action == KeyAction::PRESS ||
				keyEvent.action == KeyAction::REPEAT)
			{
				pressedKeys.setElement(i, keyEvent);
			}
			else
			{
				pressedKeys.removeElementAndSwapWithLast(i);
			}
			return;
		}
	}
	pressedKeys.appendElement(keyEvent);
}

InputHandler::InputHandler(GLFWwindow* _window)
{
	lastUpdate = std::chrono::high_resolution_clock::now();
	now = std::chrono::high_resolution_clock::now();
	window = _window;

	resetMouseInfo();

	lockMouse();
	update();
	unlockMouse();

	setMousePositionCenter();
}

void InputHandler::resetMouseInfo()
{
	mouseDeltaX = 0.0f;
	mouseDeltaY = 0.0f;
	mouseDeltaScroll = 0.0f;
}

void InputHandler::afterPushDelay()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
