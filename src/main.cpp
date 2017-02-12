/*
 * CPE 476 - Jon Catanio, Alex Ehm, Reed Garmsen
 *
 * Application code for "Granny Gauntlet"
 */

#include "ShaderHelper.h"
#include "GameWorld.h"
#include "GameManager.h"
#include "ViewFrustum.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "WindowManager.h"

#include "WallPhysicsComponent.h"
#include "WallRenderComponent.h"
#include "CookieActionComponent.h"

// Where the resources are loaded from
std::string resourceDirectory = "../resources/";
std::string RESOURCE_DIR = "../resources/";

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

// TODO(rgarmsen2295): Move into GLSL Graphics API Manager class
static void initMisc() {
    GLSL::checkVersion();

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
    shapeGirl->loadMesh(RESOURCE_DIR + "lowpolycar.obj");
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

// Sets up a simple static world/room for a simple level in Granny Gauntlet
static void setupStaticWorld(GameWorld& world) {
    
	// Floor "Wall"
	WallRenderComponent* floorRenderComp = new WallRenderComponent(shapeCube, "Phong", green);
	std::shared_ptr<GameObject> floor = std::make_shared<GameObject>(
		GameObjectType::STATIC_OBJECT, 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		0.0f, 
		glm::vec3(1000, 0.01, 1000),
		nullptr,
		nullptr,
		floorRenderComp,
        nullptr);
	floor->initComponents();
	world.addStaticGameObject(floor);

	// Both sides of the current world 'miror' each other
	for (int i = -1; i < 2; ++i) {
		if (i == 0) {
			continue;
		}

		// Cube House 1
		WallPhysicsComponent* house1PhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* house1RenderComp = new WallRenderComponent(shapeCube, "Blue", obsidian);
		std::shared_ptr<GameObject> house1 = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(-25.0f * i, 5.1f, -20.0f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f), 
			0.0f,
			glm::vec3(5, 5, 5),
			nullptr,
			house1PhysicsComp,
			house1RenderComp,
	    nullptr,
      true);
		house1->initComponents();
		world.addStaticGameObject(house1);

		// Cube House 2
		WallPhysicsComponent* house2PhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* house2RenderComp = new WallRenderComponent(shapeCube, "Blue", obsidian);
		std::shared_ptr<GameObject> house2 = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(-30.0f * i, 5.1f, 20.0f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			glm::vec3(5, 5, 5),
			nullptr,
			house2PhysicsComp,
			house2RenderComp,
	    nullptr,
      true);
		house2->initComponents();
		world.addStaticGameObject(house2);

		// Cube House 3
		WallPhysicsComponent* house3PhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* house3RenderComp = new WallRenderComponent(shapeCube, "Blue", obsidian);
		std::shared_ptr<GameObject> house3 = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(-40.0f * i, 5.1f, -20.0f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			glm::vec3(5, 5, 5),
			nullptr,
			house3PhysicsComp,
			house3RenderComp,
	    nullptr,
      true);
		house3->initComponents();
		world.addStaticGameObject(house3);

		// Cube House 4
		WallPhysicsComponent* house4PhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* house4RenderComp = new WallRenderComponent(shapeCube, "Blue", obsidian);
		std::shared_ptr<GameObject> house4 = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(-10.0f * i, 5.1f, 20.0f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			glm::vec3(5, 5, 5),
			nullptr,
			house4PhysicsComp,
			house4RenderComp,
	    nullptr,
      true);
		house4->initComponents();
		world.addStaticGameObject(house4);

		// Cube House 5
		WallPhysicsComponent* house5PhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* house5RenderComp = new WallRenderComponent(shapeCube, "Blue", obsidian);
		std::shared_ptr<GameObject> house5 = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(-60.0f * i, 5.1f, -5.0f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			glm::vec3(5, 5, 5),
			nullptr,
			house5PhysicsComp,
			house5RenderComp,
	    nullptr,
      true);
		house5->initComponents();
		world.addStaticGameObject(house5);

		// Cube House 6
		WallPhysicsComponent* house6PhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* house6RenderComp = new WallRenderComponent(shapeCube, "Blue", obsidian);
		std::shared_ptr<GameObject> house6 = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(-60.0f * i, 5.1f, 15.0f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			glm::vec3(5, 5, 5),
			nullptr,
			house6PhysicsComp,
			house6RenderComp,
	    nullptr,
      true);
		house6->initComponents();
		world.addStaticGameObject(house6);

		// Cube House 7
		WallPhysicsComponent* house7PhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* house7RenderComp = new WallRenderComponent(shapeCube, "Blue", obsidian);
		std::shared_ptr<GameObject> house7 = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(-60.0f * i, 5.1f, 35.0f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			glm::vec3(5, 5, 5),
			nullptr,
			house7PhysicsComp,
			house7RenderComp,
	    nullptr,
      true);
		house7->initComponents();
		world.addStaticGameObject(house7);

		// Cube House 8
		WallPhysicsComponent* house8PhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* house8RenderComp = new WallRenderComponent(shapeCube, "Blue", obsidian);
		std::shared_ptr<GameObject> house8 = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(-30.0f * i, 5.1f, 45.0f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			glm::vec3(5, 5, 5),
			nullptr,
			house8PhysicsComp,
			house8RenderComp,
	    nullptr,
      true);
		house8->initComponents();
		world.addStaticGameObject(house8);

		// Draw walls on opposite ends of map
		WallPhysicsComponent* shortWall1PhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* shortWall1RenderComp = new WallRenderComponent(shapeCube, "Green", obsidian);
		std::shared_ptr<GameObject> shortWall1 = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(-40.0f * i, 0.5f, 45.0f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			glm::vec3(0.75, 1.0, 35),
			nullptr,
			shortWall1PhysicsComp,
			shortWall1RenderComp,
	        nullptr);
		shortWall1->initComponents();
		world.addStaticGameObject(shortWall1);

		WallPhysicsComponent* shortWall2PhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* shortWall2RenderComp = new WallRenderComponent(shapeCube, "Green", obsidian);
		std::shared_ptr<GameObject> shortWall2 = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(-50.0f * i, 0.5f, 35.0f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			glm::vec3(0.75, 1.0, 45),
			nullptr,
			shortWall2PhysicsComp,
			shortWall2RenderComp,
	        nullptr);
		shortWall2->initComponents();
		world.addStaticGameObject(shortWall2);

		// Draw wall along center driveway
		WallPhysicsComponent* longWallPhysicsComp = new WallPhysicsComponent();
		WallRenderComponent* longWallRenderComp = new WallRenderComponent(shapeCube, "Green", obsidian);
		std::shared_ptr<GameObject> longWall = std::make_shared<GameObject>(
			GameObjectType::STATIC_OBJECT, 
			glm::vec3(5.0f * i, 0.5f, 10.75f * i), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			glm::vec3(45.5, 1.0, 0.75),
			nullptr,
			longWallPhysicsComp,
			longWallRenderComp,
	        nullptr);
		longWall->initComponents();
		world.addStaticGameObject(longWall);
	}

	// Back wall to start location
	WallPhysicsComponent* startWallPhysicsComp = new WallPhysicsComponent();
	WallRenderComponent* startWallRenderComp = new WallRenderComponent(shapeCube, "Green", obsidian);
	std::shared_ptr<GameObject> startWall = std::make_shared<GameObject>(
		GameObjectType::STATIC_OBJECT, 
		glm::vec3(45.0f, 0.5f, -79.25f), 
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f,
		glm::vec3(5, 1.0, 0.75),
		nullptr,
		startWallPhysicsComp,
		startWallRenderComp,
        nullptr);
	startWall->initComponents();
	world.addStaticGameObject(startWall);

	// 'Finish' wall
	WallPhysicsComponent* finishWallPhysicsComp = new WallPhysicsComponent();
	WallRenderComponent* finishWallRenderComp = new WallRenderComponent(shapeCube, "Red", obsidian);
	std::shared_ptr<GameObject> finishWall = std::make_shared<GameObject>(
		GameObjectType::STATIC_OBJECT, 
		glm::vec3(-45.0f, 0.5f, 79.25f), 
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f,
		glm::vec3(5, 1.0, 0.75),
		nullptr,
		finishWallPhysicsComp,
		finishWallRenderComp,
        nullptr);
	finishWall->initComponents();
	world.addStaticGameObject(finishWall);
}

int main(int argc, char **argv) {

	// Parse commandline arguments and check for errors
	if (parseArgs(argc, argv) == -1) {
		return EXIT_FAILURE;
	}

	// Initialize boilerplate glfw, etc. code and check for failure
    WindowManager& windowManager = WindowManager::instance();    
    if (windowManager.initialize() == -1) {
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

	// Load temporary hard color shaders
	if (shaderManager.createIsomorphicShader(resourceManager, "Blue", "dummyBlue") == 0) {
		return EXIT_FAILURE;
	}

	if (shaderManager.createIsomorphicShader(resourceManager, "Green", "dummyGreen") == 0) {
		return EXIT_FAILURE;
	}

	if (shaderManager.createIsomorphicShader(resourceManager, "Red", "dummyRed") == 0) {
		return EXIT_FAILURE;
	}

   PlayerInputComponent* playerInputComp = new PlayerInputComponent();
   PlayerPhysicsComponent* playerPhysicsComp = new PlayerPhysicsComponent();
    CookieActionComponent* cookieAction = new CookieActionComponent();
   PlayerRenderComponent* playerRenderComp = new PlayerRenderComponent(shapeGirl,
      "Phong", pearl);

    // The current game world
    GameWorld world;

    // Initialize the GameManager and get its instance
    GameManager& gameManager = GameManager::instance();

    // Set the manager to the current game world
    gameManager.setGameWorld(&world);

   std::shared_ptr<GameObject> player = std::make_shared<GameObject>(
      GameObjectType::PLAYER,
      glm::vec3(45.0f, 1.0f, -70.0f),
      glm::vec3(-1.0f, 0.0f, 0.0f),
      12.0f,
      glm::vec3(1.0f, 1.0f, 1.0f),
      playerInputComp,
      playerPhysicsComp,
      playerRenderComp,
	  cookieAction
   );
   player->initComponents();

   /* Set the orient angle to orient the object correctly from it's starting pos.
    * This is specific to each obj file. Positive values are cw, negative ccw */ 
   player->setYAxisRotation(-M_PI / 4);
   player->setOrientAngle(-M_PI / 4);

	// The current game camera
	Camera camera(player);

   world.addDynamicGameObject(player);

	// Set the manager to the current camera
    gameManager.setCamera(&camera);

    // Set the manager to the current player object
    gameManager.setPlayer(player);

    // Set the current view frustum
    gameManager.setViewFrustum(new ViewFrustum());

    setupStaticWorld(world);

    // Add all static objects before this!!!
    world.init();

    gameManager.setTime(30.0);

    // Loop until the user closes the window
    int numFramesInSecond = 0;
    constexpr double dt = 1.0 / 60.0;
    double totalTime = 0.0;
    double secondClock = 0.0;
    double startTime = glfwGetTime();
    double previousTime = startTime;

    while (!windowManager.isClosed()) {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - previousTime;
        previousTime = currentTime;

        // Poll for and process events
        windowManager.pollEvents();

        while (elapsedTime > 0.0) {
            double deltaTime = std::min(elapsedTime, dt);

            // Update all game objects
            world.updateGameObjects(deltaTime, totalTime);
            camera.update(deltaTime);

            gameManager.decreaseTime(deltaTime);

            elapsedTime -= deltaTime;
            totalTime += deltaTime;
            secondClock += deltaTime;
        }

        // Update the window in-case of resizing, etc.
        windowManager.update();

        // Draw all objects in the world
        world.drawGameObjects();

        // Swap front and back buffers
        windowManager.swapBuffers();

        // Print info roughly every second
        numFramesInSecond++;
        if (secondClock >= 1.0) {

            // TODO(rgarmsen2295): Currently acting kinda funky on linux and not 
            // really necessary so disabling for now
            // gameManager.printInfoToConsole(numFramesInSecond / secondClock);

            secondClock = 0.0;
            numFramesInSecond = 0;
        }
    }

    return EXIT_SUCCESS;
}
