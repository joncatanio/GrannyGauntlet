#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "GameManager.h"
#include "ShaderManager.h"

class WindowManager {
public:

	~WindowManager();

	// Returns the single instance of the WindowManager
	static WindowManager& instance();

	// Initializes general GLFW code and creates the actual window
	int initialize();

	// Does any necessary event polling, including keyboard and window events
	void pollEvents();

	// Updates the window according to any changes processed during run-time
	void update();

	// Checks for user setting changes and updates the game state accordingly
	void checkForUserChanges();

	// Swap front and back buffers
	void swapBuffers();

	// Checks whether or not the window was closed by the user
	bool isClosed();

	// Checks if the passed key has been pressed (GLFW_PRESS)
	bool isKeyPressed(int keyToCheck);

	// Returns the current view/window width in pixels
	int getViewWidth();

	// Returns the current view/window height in pixels
	int getViewHeight();

	// Returns the current X value of the cursor location
	float getCursorX();

	// Returns the current Y value of the cursor location
	float getCursorY();

	// Returns the current aspect ratio of the view/window
	float getAspectRatio();

	// Updates the current view size in pixels
	void updateViewSize(int newWidth, int newHeight);

	// Updates the current cursor position to a new location
	void updateCursorPosition(float newX, float newY);

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

	// Initializes general boiler-plate GLFW code
	int initializeGLFW();
};

#endif
