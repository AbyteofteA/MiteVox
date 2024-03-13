#include "InputHandler.h"

#include "dependencies/glfw-3.3.2.bin.WIN32/include/GLFW/glfw3.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <cassert>

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

void InputHandler::init(int screenWidth, int screenHeight, bool isFullScreen)
{
	int glfwInitStatus = glfwInit();
	assert(glfwInitStatus);

	std::lock_guard<std::mutex> lock(mutex);

	if (instance == nullptr)
	{
		instance = new InputHandler();
		instance->pressedKeys.reserve(16);
		instance->keyEventQueue.reserve(1024);
	}

	instance->createWindow(screenWidth, screenHeight, false);
	instance->lockMouse();
	instance->update();
	instance->unlockMouse();
	instance->setMousePositionCenter();
}

InputHandler* InputHandler::getInstance()
{
	return instance;
}

void InputHandler::pollEvents()
{
	glfwPollEvents();
}

void InputHandler::createWindow(int screenWidth, int screenHeight, bool isFullScreen)
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_SAMPLES, 1);

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);

	if (isFullScreen)
	{
		window = glfwCreateWindow(screenWidth, screenHeight, "MiteVox", monitor, nullptr);
	}
	else
	{
		window = glfwCreateWindow(screenWidth, screenHeight, "MiteVox", nullptr, nullptr);
	}

	if (window)
	{
		// TODO: logger.logInfo("EngineSettings", "Window is created.");
		// TODO: logger.logInfo("EngineSettings", "Vendor: " + render::getVendorName());
		// TODO: logger.logInfo("EngineSettings", "Renderer: " + render::getRendererName());
		// TODO: logger.logInfo("EngineSettings", "Version: " + render::getVersion());
		// TODO: logger.logInfo("EngineSettings", "Language Version: " + render::getLanguageVersion());
	}
	else
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	//glfwSetCursorPosCallback(instance->window, cursor_position_callback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetKeyCallback(window, keyCallback);
}

void InputHandler::display()
{
	glfwSwapBuffers(window);
}

int InputHandler::windowShouldClose()
{
	return glfwWindowShouldClose(window);
}

void InputHandler::closeWindow()
{
	glfwSetWindowShouldClose(window, true);
}

void InputHandler::getWindowSize(int& x, int& y)
{
	glfwGetWindowSize(window, &x, &y);
}

void InputHandler::setWindowSize(int x, int y)
{
	glfwSetWindowSize(window, x, y);
}

void InputHandler::getMousePosition(double& x, double& y)
{
	glfwGetCursorPos(window, &x, &y);
}

void InputHandler::setMousePosition(double x, double y)
{
	glfwSetCursorPos(window, x, y);
}

void InputHandler::setMousePositionCenter()
{
	int windowWidth, windowHeight;
	getWindowSize(windowWidth, windowHeight);
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
	getWindowSize(windowWidth, windowHeight); // TODO: store windowWidth, windowHeight

	double currentMousePositionX = 0.0, currentMousePositionY = 0.0f;
	getMousePosition(currentMousePositionX, currentMousePositionY);
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

InputHandler::InputHandler()
{
	lastUpdate = std::chrono::high_resolution_clock::now();
	now = std::chrono::high_resolution_clock::now();

	resetMouseInfo();
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
