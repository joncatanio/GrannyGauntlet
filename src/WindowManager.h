#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "GameManager.h"

class WindowManager {
public:

	~WindowManager();

	// Returns the single instance of the WindowManager
	static WindowManager& instance();

	int initialize();

	void pollEvents();

	void update();

	// Swap front and back buffers
	void swapBuffers();

	bool isClosed();

	bool isKeyPressed(int keyToCheck);

	int getViewWidth();

	int getViewHeight();

	float getCursorX();

	float getCursorY();

	float getAspectRatio();

	void updateViewSize(int newWidth, int newHeight);

	void updateCursorPosition(float newX, float newY);

	int initializeGLFW();

private:

	// Main application window (left as a normal pointer for easier use with GLFW functions)
	GLFWwindow* window_;

	// Width and height variables representing the view resolution
	int width_;
	int height_;

	// Current x & y pos for the mouse cursor
	float currentCursorX_;
	float currentCursorY_;

	// The current aspect ratio of the view
	float aspect_;

	WindowManager();

	void updateFramebuffer();

};

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

static void mouseCallback(GLFWwindow* window, int button, int action, int mods);

static void cursorPositionCallback(GLFWwindow* window, double posX, double posY);

static void resizeCallback(GLFWwindow* window, int width, int height);

static void errorCallback(int error, const char* description);

#endif
