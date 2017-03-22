#include "WindowManager.h"
#include "GameManager.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "TextureManager.h"

WindowManager::~WindowManager() {
	glfwDestroyWindow(window_);
   ImGui_ImplGlfwGL3_Shutdown();
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

   updateGui();
}

void WindowManager::updateGui() {
   ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

   ImGui_ImplGlfwGL3_NewFrame();

   // Needed for changing the style of individual elements, see ImGuiCol_ in imgui.h
   ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
   //ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.992, 0.376, 0.380, 1));
   ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));

   if (score_window) {
      // Display the score
      ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("score_window", &score_window, flags);
      ImGui::Text("SCORE: %d", static_cast<int>(GameManager::instance().getScore()));
      ImGui::End();

   }

   if (time_window) {
      // Display the time
      ImGui::SetNextWindowPos(ImVec2(25, 65), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("time_window", &time_window, flags);
      ImGui::Text("TIME REMAINING: %d", static_cast<int>(GameManager::instance().getTime()));
      ImGui::End();
   }

   // Pop the style
   ImGui::PopStyleColor(2);
}

void WindowManager::checkForUserChanges() {
    GameManager& gameManager = GameManager::instance();
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
        //textureManager.increaseTextureToggle();
        gameManager.enterHell();
        mWasPressed = true;
    }
    if (!isKeyPressed(GLFW_KEY_M)) {
        mWasPressed = false;
    }

    if (!menuKeyWasPressed && isKeyPressed(GLFW_KEY_ESCAPE)) {
        // Should close the GLFW window
        //glfwSetWindowShouldClose(window_, GL_TRUE);

        menuKeyWasPressed = true;

        if(!gameManager.getMenu()->isActive()) {
            gameManager.getMenu()->toggleMenuActive();
            gameManager.getMenu()->menuStartTime_ = glfwGetTime();
        } else {
            if(gameManager.getMenu()->isInPause()) {
                gameManager.getMenu()->toggleMenuActive();
                gameManager.getMenu()->leftMenuThisFrame_ = true;
                gameManager.getMenu()->menuTime_ = glfwGetTime() - gameManager.getMenu()->menuStartTime_;
            }
        }
    }
    if (menuKeyWasPressed && !isKeyPressed(GLFW_KEY_ESCAPE)) {
        menuKeyWasPressed = false;
    }


    if(gameManager.getMenu()->isActive()) {
        if(isKeyPressed(GLFW_KEY_UP) && !upWasPressed){
            gameManager.getMenu()->selectedItemDown();
            upWasPressed = true;
        }
        if(!isKeyPressed(GLFW_KEY_UP)) {
            upWasPressed = false;
        }

        if(isKeyPressed(GLFW_KEY_DOWN) && !downWasPressed) {
            gameManager.getMenu()->selectedItemUp();
            downWasPressed = true;
        }
        if(!isKeyPressed(GLFW_KEY_DOWN)) {
            downWasPressed = false;
        }

        if(isKeyPressed(GLFW_KEY_ENTER) && !spaceWasPressed) {
            gameManager.getMenu()->performMenuAction();
            spaceWasPressed = true;
        }

        if(!isKeyPressed(GLFW_KEY_ENTER)) {
            spaceWasPressed = false;
        }

    }



}

void WindowManager::swapBuffers() {
   ImGui::Render();
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
	currentCursorY_(0),
   score_window(true),
   time_window(true) {

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

    // ImGui Initiliazation
    ImGui_ImplGlfwGL3_Init(window_, true);
    ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontFromFileTTF("../resources/fonts/Roboto-Medium.ttf", 35.0f);
    io.Fonts->AddFontFromFileTTF("../resources/fonts/sketch3d.otf", 40.0f);

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

void WindowManager::close() {
    glfwSetWindowShouldClose(window_, GL_TRUE);
}
