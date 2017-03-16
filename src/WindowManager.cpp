#include "WindowManager.h"

#include "TextureManager.h"

WindowManager::~WindowManager() {
	glfwDestroyWindow(window_);
	glfwTerminate();
}

WindowManager& WindowManager::instance() {
	static WindowManager *instance = new WindowManager();
	return *instance;
}

int initializeGLFW(GLFWwindow** window);

int WindowManager::initialize() {
	// Initialize boilerplate glfw, etc. code and check for failure
    if (initializeGLFW() == -1) {
        return -1;
    }

	double posX, posY;
	glfwGetCursorPos(window_, &posX, &posY);
	updateCursorPosition(posX, posY);

	return 0;
}

void WindowManager::pollEvents() {
	glfwPollEvents();
}

void WindowManager::update() {
	updateFramebuffer();
	aspect_ = getAspectRatio();

    checkForUserChanges();
}

void WindowManager::checkForUserChanges() {
    ShaderManager& shaderManager = ShaderManager::instance();
    TextureManager& textureManager = TextureManager::instance();

    // Check for shader setting changes
    if (isKeyPressed(GLFW_KEY_P)) {
        // Use the "Phong" shader as the current default
        shaderManager.setDefaultShader(shaderManager.PhongShader);

        std::cout << "Switched to " << shaderManager.PhongShader << " shader!" << std::endl;
    } else if (isKeyPressed(GLFW_KEY_C)) {
        // Use the "CookTorrance" shader as the current default
        shaderManager.setDefaultShader(shaderManager.CookTorranceShader);

        std::cout << "Switched to " << shaderManager.CookTorranceShader << " shader!" << std::endl;
    } else if (isKeyPressed(GLFW_KEY_T)) {
        // Use the "Toon" shader as the current default
        shaderManager.setDefaultShader(shaderManager.ToonShader);

        std::cout << "Switched to " << shaderManager.ToonShader << " shader!" << std::endl;
    }

    if (isKeyPressed(GLFW_KEY_M) && !mWasPressed) {
        textureManager.increaseTextureToggle();
        mWasPressed = true;
    }
    if (!isKeyPressed(GLFW_KEY_M)) {
        mWasPressed = false;
    }

    if (isKeyPressed(GLFW_KEY_ESCAPE)) {
        // Should close the GLFW window
        glfwSetWindowShouldClose(window_, GL_TRUE);
    }
}

void WindowManager::swapBuffers() {
	glfwSwapBuffers(window_);
}

bool WindowManager::isClosed() {;
	return glfwWindowShouldClose(window_);
}

bool WindowManager::isKeyPressed(int keyToCheck) {
	return glfwGetKey(window_, keyToCheck) == GLFW_PRESS;
}

int WindowManager::getViewWidth() {
	return width_;
}

int WindowManager::getViewHeight() {
	return height_;
}

float WindowManager::getCursorX() {
	return currentCursorX_;
}

float WindowManager::getCursorY() {
	return currentCursorY_;
}

float WindowManager::getAspectRatio() {
	return aspect_;
}

void WindowManager::updateViewSize(int newWidth, int newHeight) {
	width_ = newWidth;
	height_ = newHeight;

	aspect_ = width_ / (float) height_;
}

void WindowManager::updateCursorPosition(float newX, float newY) {
	currentCursorX_ = newX;
	currentCursorY_ = newY;
}

WindowManager::WindowManager()
	: currentCursorX_(0),
	currentCursorY_(0) {

}

void WindowManager::updateFramebuffer() {

	// Get current frame buffer size
	int newWidth, newHeight;
	glfwGetFramebufferSize(window_, &newWidth, &newHeight);
	glViewport(0, 0, newWidth, newHeight);
	updateViewSize(newWidth, newHeight);

	// Clear the framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Various callbacks that are set within |initializeGLFW| to be called upon user action
// when necessary by GLFW
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
static void cursorPositionCallback(GLFWwindow* window, double posX, double posY);
static void resizeCallback(GLFWwindow* window, int width, int height);
static void errorCallback(int error, const char* description);

int WindowManager::initializeGLFW() {
    // Set error callback.
    glfwSetErrorCallback(errorCallback);

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
    window_ = glfwCreateWindow(1920, 1080, "Granny Gauntlet", NULL, NULL);
    if (!window_) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current.
    glfwMakeContextCurrent(window_);

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
    glfwSetKeyCallback(window_, keyCallback);

    // Set sticky keys for more accurate polling
    glfwSetInputMode(window_, GLFW_STICKY_KEYS, 1);

    // Set the mouse call back
    glfwSetMouseButtonCallback(window_, mouseCallback);

    // Set cursor position call back for camera movement
    glfwSetCursorPosCallback(window_, cursorPositionCallback);

    // Turn-off cursor mode (lock mouse control to window for camera movement)
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the window resize call back
    glfwSetFramebufferSizeCallback(window_, resizeCallback);

    return 0;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Nothing here for now
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    // Nothing here for now
}

static void cursorPositionCallback(GLFWwindow* window, double posX, double posY) {
#ifdef ENABLE_MOUSE
	WindowManager& windowManager = WindowManager::instance();
    GameManager& gameManager = GameManager::instance();
    Camera& camera = gameManager.getCamera();

    float deltaAlphaScale = 180.0f / windowManager.getViewHeight();
    float deltaBetaScale = 360.0f / windowManager.getViewWidth();

    float deltaX = posX - windowManager.getCursorX();
    float deltaY = -(posY - windowManager.getCursorY()); // Negative to prevent inverted control

    windowManager.updateCursorPosition(posX, posY);

    // Get the change in angle depending on the width / height of the view
    float deltaAlpha = deltaY * deltaAlphaScale;
    float deltaBeta = deltaX * deltaBetaScale;

    camera.changeAlpha(deltaAlpha);
    camera.changeBeta(deltaBeta);
#endif
}

static void resizeCallback(GLFWwindow* window, int width, int height) {
	WindowManager& windowManager = WindowManager::instance();
	windowManager.updateViewSize(width, height);

    glViewport(0, 0, width, height);
}

static void errorCallback(int error, const char* description) {
    std::cerr << description << std::endl;
}
