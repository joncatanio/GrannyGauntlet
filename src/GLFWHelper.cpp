#include "GLFWHelper.h"

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		// Should close the GLFW window
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

static void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
	// Nothing here for now
}

static void cursor_pos_callback(GLFWwindow* window, double posX, double posY) {
	GameManager& gameManager = GameManager::instance();
	Camera& camera = gameManager.getCamera();

	float deltaAlphaScale = 180.0f / g_height;
	float deltaBetaScale = 360.0f / g_width;

	float deltaX = posX - prevX;
	float deltaY = -(posY - prevY); // Negative to prevent inverted control

	prevX = posX;
	prevY = posY;

	// Get the change in angle depending on the width / height of the view
	float deltaAlpha = deltaY * deltaAlphaScale;
	float deltaBeta = deltaX * deltaBetaScale;

	camera.changeAlpha(deltaAlpha);
	camera.changeBeta(deltaBeta);
}

static void resize_callback(GLFWwindow *window, int width, int height) {
	g_width = width;
	g_height = height;
	glViewport(0, 0, width, height);
}

static void error_callback(int error, const char *description) {
	std::cerr << description << std::endl;
}

int initializeGLFW(GLFWwindow **window) {
	// Set error callback.
	glfwSetErrorCallback(error_callback);

	// Initialize the library.
	if (!glfwInit()) {
		return -1;
	}

	//request the highest possible version of OpenGL - important for mac
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	// Create a windowed mode window and its OpenGL context.
	*window = glfwCreateWindow(600, 480, "Granny Gauntlet", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current.
	glfwMakeContextCurrent(*window);

	// Initialize GLEW.
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Weird bootstrap of glGetError
	glGetError();
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Set vsync.
	glfwSwapInterval(1);

	// Set keyboard callback.
	glfwSetKeyCallback(*window, key_callback);

   // Set sticky keys for more accurate polling
   glfwSetInputMode(*window, GLFW_STICKY_KEYS, 1);

	// Set the mouse call back
	glfwSetMouseButtonCallback(*window, mouse_callback);

	// Set cursor position call back for camera movement
	glfwSetCursorPosCallback(*window, cursor_pos_callback);

	// Turn-off cursor mode (lock mouse control to window for camera movement)
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the window resize call back
	glfwSetFramebufferSizeCallback(*window, resize_callback);

	return 0;
}

// Call before exiting the program
void cleanupGLFW(GLFWwindow **window) {
	glfwDestroyWindow(*window);
	glfwTerminate();
}
