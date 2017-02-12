/*
 * CPE 476 - Jon Catanio, Alex Ehm, Reed Garmsen
 *
 * Application code for "Granny Gauntlet"
 */

#include "ShaderHelper.h"
#include "LevelLoader.h"
#include "GameWorld.h"
#include "GameManager.h"
#include "ViewFrustum.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "ShapeManager.h"
#include "MaterialManager.h"
#include "WindowManager.h"

#include "WallPhysicsComponent.h"
#include "WallRenderComponent.h"
#include "CookieActionComponent.h"

// Where the resources are loaded from
std::string resourceDirectory = "../resources/";
std::string RESOURCE_DIR = "../resources/";

// TODO(rgarmsen2295): Move into GLSL Graphics API Manager class
static void initMisc() {
    GLSL::checkVersion();

    // Set background color
    glClearColor(0.25f, 0.875f, 0.924f, 1.0f);

    // Enable z-buffer test
    glEnable(GL_DEPTH_TEST);
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

	// Initialize the ResourceManager and get its instance
	ResourceManager& resourceManager = ResourceManager::instance();
	resourceManager.setResourceDirectory(resourceDirectory);
   ShapeManager& shapeManager = ShapeManager::instance();
   MaterialManager& materialManager = MaterialManager::instance();

   // Instantiate the current game world and load it.
   GameWorld world;
   LevelLoader& levelLoader = LevelLoader::instance();
   if (levelLoader.loadLevel(world)) {
      std::cerr << "Error loading level." << std::endl;
      return EXIT_FAILURE;
   }

   PlayerInputComponent* playerInputComp = new PlayerInputComponent();
   PlayerPhysicsComponent* playerPhysicsComp = new PlayerPhysicsComponent();
    CookieActionComponent* cookieAction = new CookieActionComponent();
   PlayerRenderComponent* playerRenderComp = new PlayerRenderComponent(
      shapeManager.getShape("Lowpolycar"),
      "Phong", materialManager.getMaterial("Pearl"));


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

    //setupStaticWorld(world);

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
