/*
 * CPE 476, Lab 1 - Reed Garmsen
 * Beginnings of a Game Engine
 */

#include "ShaderHelper.h"
#include "GameWorld.h"
#include "GameManager.h"
#include "GLFWHelper.h"
#include "ResourceManager.h"
#include "ShaderManager.h"

#include "WallRenderComponent.h"

// Global height and width variables representing the view resolution
int g_width, g_height;

// Aspect ratio of the window
float aspect;

// Previous x & y pos for the mouse
float prevX = 0;
float prevY = 0;

// Where the resources are loaded from
std::string resourceDirectory = "../resources/";
std::string RESOURCE_DIR = "../resources/";

// Main application window
GLFWwindow *window;

// Material pointers
// TODO(rgarmsen2295): Move into shader manager class
std::shared_ptr<Material> obsidian;
std::shared_ptr<Material> green;
std::shared_ptr<Material> jade;
std::shared_ptr<Material> pearl;
std::shared_ptr<Material> brass;

// Shape pointers
// TODO(rgarmsen2295): Move into shader manager class
std::shared_ptr<Shape> shapeCube;
std::shared_ptr<Shape> shapeGirl;

// Main directional light properties
// TODO(rgarmsen2295): Move into shader manager class
//Light curLight = {-1.75, 10, -1.75, 1, 1, 1, 0, 1};
Light curLight = { 0, 10, 0, 1, 1, 1, 0, 1 };

// TODO(rgarmsen2295): Move into GLSL Graphics API Manager class
static void initMisc() {
	GLSL::checkVersion();

	double posX, posY;
	glfwGetCursorPos(window, &posX, &posY);
	prevX = posX;
	prevY = posY;

	// Set background color
	glClearColor(0.25f, 0.875f, 0.924f, 1.0f);

	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
}

// TODO(rgarmsen2295): Move into GeometryManager class
static void initGeometry() {

	// Initialize the cube mesh
	shapeCube = std::make_shared<Shape>();
	shapeCube->loadMesh(RESOURCE_DIR + "cube.obj");
	shapeCube->resize();
	shapeCube->init();

	shapeGirl = std::make_shared<Shape>();
	shapeGirl->loadMesh(RESOURCE_DIR + "girl.obj");
	shapeGirl->resize();
	shapeGirl->init();
}

// Values sourced from - http://devernay.free.fr/cours/opengl/materials.html
// TODO(rgarmsen2295): Move into MaterialManager or ShaderManager class
static void initMaterials() {

	// Initialize the obsidian material
	obsidian = std::make_shared<Material>();
	*obsidian = { 0.05375f, 0.05f, 0.06625f, 0.18275f, 0.17f, 0.22525f, 0.332741f, 0.328634f, 0.346435f, 38.4f };

	// Changed to be less "normal" green
	green = std::make_shared<Material>();
	*green = { 0.4f, 0.7f, 0.4f, 0.4f, 1.0f, 0.1f, 0.0225f, 0.0225f, 0.0225f, 12.8f };

	// modified shininess from given value (0.1f)
	jade = std::make_shared<Material>();
	*jade = { 0.135f, 0.2225f, 0.1575f, 0.54f, 0.89f, 0.63f, 0.316228f, 0.316228f, 0.316228f, 10.0f };

	pearl = std::make_shared<Material>();
	*pearl = { 0.25f, 0.20725f, 0.20725f, 1.0f, 0.829f, 0.296648f, 0.296648f, 0.296648f, 0.296648f, 0.088f };

	// modified shininess from given value (0.21794872f)
	brass = std::make_shared<Material>();
	*brass = { 0.329412f, 0.223529f, 0.027451f, 0.780392f, 0.568627f, 0.113725f, 0.992157f, 0.941176f, 0.807843f, 10.0f };
}

static void updateFrameBuffer() {

	// Get current frame buffer size
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	g_width = width;
	g_height = height;

	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static float getAspectRatio() {
	return g_width / (float) g_height;
}

int parseArgs(int argc, char **argv) {
#ifdef linux
	// Assume default resource directory if none is given
	if (argc >= 2) {
		RESOURCE_DIR = argv[1] + string("/");
	}
#elif _WIN32
	// Don't even try to guess default directory with Windows
	if (argc < 2) {
		std::cerr << "Error: No specified resource directory in arguments" << std::endl;
		std::cerr << "Expected: ./a.out [RESOURCE_DIR]" << std::endl;
		return -1;
	}

	RESOURCE_DIR = argv[1] + std::string("/");
#endif

	return 0;
}

// Sets up a simple static world/room used in Lab 1 for CPE 476
static void setupStaticWorld(GameWorld& world) {
	ShaderManager& shaderManager = ShaderManager::instance();
	std::shared_ptr<Program> progPhong = shaderManager.getShaderProgram("Phong");

	// Floor "Wall"
	WallRenderComponent* floorRenderComp = new WallRenderComponent(shapeCube, progPhong, green);
	GameObject* floor = new GameObject(
		GameObjectType::STATIC_OBJECT, 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		0.0f, 
		glm::vec3(30, 0.01, 30),
		NULL,
		NULL,
		floorRenderComp);
	world.addStaticGameObject(floor);
	
	// Left Wall
	WallRenderComponent* leftWallRenderComp = new WallRenderComponent(shapeCube, progPhong, obsidian);
	GameObject* leftWall = new GameObject(
		GameObjectType::STATIC_OBJECT, 
		glm::vec3(-30.0f, 0.0f, 0.0f), 
		glm::vec3(1.0f, 0.0f, 0.0f), 
		0.0f, 
		glm::vec3(0.01, 30, 30),
		NULL, 
		NULL, 
		leftWallRenderComp);
	world.addStaticGameObject(leftWall);

	// Right Wall
	WallRenderComponent* rightWallRenderComp = new WallRenderComponent(shapeCube, progPhong, obsidian);
	GameObject* rightWall = new GameObject(
		GameObjectType::STATIC_OBJECT,
		glm::vec3(30.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		0.0f,
		glm::vec3(0.01, 30, 30),
		NULL,
		NULL,
		rightWallRenderComp);
	world.addStaticGameObject(rightWall);

	// Front Wall
	WallRenderComponent* frontWallRenderComp = new WallRenderComponent(shapeCube, progPhong, obsidian);
	GameObject* frontWall = new GameObject(
		GameObjectType::STATIC_OBJECT,
		glm::vec3(0.0f, 0.0f, -30.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		0.0f,
		glm::vec3(30, 30, 0.01),
		NULL,
		NULL,
		frontWallRenderComp);
	world.addStaticGameObject(frontWall);

	// Back Wall
	WallRenderComponent* backWallRenderComp = new WallRenderComponent(shapeCube, progPhong, obsidian);
	GameObject* backWall = new GameObject(
		GameObjectType::STATIC_OBJECT,
		glm::vec3(0.0f, 0.0f, 30.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		0.0f,
		glm::vec3(30, 30, 0.01),
		NULL,
		NULL,
		backWallRenderComp);
	world.addStaticGameObject(backWall);
}

int main(int argc, char **argv) {

	// Parse commandline arguments and check for errors
	if (parseArgs(argc, argv) == -1) {
		return EXIT_FAILURE;
	}

	// Initialize boilerplate glfw, etc. code and check for failure
	if (initializeGLFW(&window) == -1) {
		return EXIT_FAILURE;
	}

	// TODO(rgarmsen2295): Move into some central manager class
	// Initialize scene data
	initMisc();
	initGeometry();
	initMaterials();

	// Initialize the ResourceManager and get its instance
	ResourceManager& resourceManager = ResourceManager::instance();
	resourceManager.setResourceDirectory(resourceDirectory);

	// Initialize the ShaderManager and get its instance
	ShaderManager& shaderManager = ShaderManager::instance();

	// Load a phong shader
	if (shaderManager.createIsomorphicShader(resourceManager, "Phong", "phong") == 0) {
		return EXIT_FAILURE;
	}

   PlayerInputComponent* playerInputComp = new PlayerInputComponent();
   PlayerPhysicsComponent* playerPhysicsComp = new PlayerPhysicsComponent();
   PlayerRenderComponent* playerRenderComp = new PlayerRenderComponent(shapeGirl,
      shaderManager.getShaderProgram("Phong"), pearl);
   GameObject* player = new GameObject(
      GameObjectType::NONSTATIC_OBJECT,
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::vec3(-1.0f, 0.0f, 0.0f),
      12.0f,
      glm::vec3(1.0f, 1.0f, 1.0f),
      playerInputComp,
      playerPhysicsComp,
      playerRenderComp
   );
   player->setYAxisRotation(-M_PI / 2);

	// The current game camera
	Camera camera(player);

	// The current game world
	GameWorld world;
   world.addNonStaticGameObject(player);

	// Initialize the GameManager and get its instance
	GameManager& gameManager = GameManager::instance();

	// Set the manager to the current game world
	gameManager.setGameWorld(&world);

	// Set the manager to the current camera
	gameManager.setCamera(&camera);

   // Set the manager to the current player object
   gameManager.setPlayer(player);

	setupStaticWorld(world);

	// Loop until the user closes the window
	int numFramesInSecond = 0;
	constexpr double dt = 1.0 / 60.0;
	double totalTime = 0.0;
	double secondClock = 0.0;
	double startTime = glfwGetTime();
	double previousTime = startTime;

	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		double elapsedTime = currentTime - previousTime;
		previousTime = currentTime;

		// Poll for and process events
		// TODO(rgarmsen2295): Remove and switch to a more maintainable polling system
		glfwPollEvents();

		while (elapsedTime > 0.0) {
			double deltaTime = std::min(elapsedTime, dt);

			// Update all game objects
			world.updateGameObjects(deltaTime, totalTime);
			camera.update(deltaTime);

			elapsedTime -= deltaTime;
			totalTime += deltaTime;
			secondClock += deltaTime;
		}

		// Render scene

		// Update the frame buffer in-case of resizing, etc.
		// TODO(rgarmsen2295): Remove and refactor out to WindowManager
		updateFrameBuffer();

		// Get the current aspect ratio
		// TODO(rgarmsen2295): Remove and refactor out to WindowManager
		aspect = getAspectRatio();

		// Draw all objects in the world
		world.drawGameObjects();

		// Swap front and back buffers
		glfwSwapBuffers(window);

		numFramesInSecond++;

		// Print info roughly every second
		if (secondClock >= 1.0) {

			// TODO(rgarmsen2295): Currently acting kinda funky on linux and not really necessary so disabling for now
			// gameManager.printInfoToConsole(numFramesInSecond / secondClock);

			secondClock = 0.0;
			numFramesInSecond = 0;
		}
	}

	// Quit program
	cleanupGLFW(&window);
	return EXIT_SUCCESS;
}
