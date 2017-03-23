/*
 * CPE 476 - Jon Catanio, Alex Ehm, Reed Garmsen
 *
 * Application code for "Granny Gauntlet"
 */

#include "LevelLoader.h"
#include "GameWorld.h"
#include "GameManager.h"
#include "ViewFrustum.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "ShaderManager.h"
#include "ShapeManager.h"
#include "MaterialManager.h"
#include "WindowManager.h"
#include "ShadowMap.h"

#include "BillboardRenderComponent.h"
#include "MenuBillboardPhysicsComponent.h"
#include "TextureManager.h"

#include "WallPhysicsComponent.h"
#include "WallRenderComponent.h"
#include "CookieActionComponent.h"

#ifdef _WIN32
#include <gl\gl.h>
#pragma comment(lib, "opengl32.lib")
#endif


// Where the resources are loaded from
std::string resourceDirectory = "../resources/";

// TODO(rgarmsen2295): Move into GLSL Graphics API Manager class
static void initMisc() {
    GLSL::checkVersion();

    // Set background color
    glClearColor(0.25f, 0.875f, 0.924f, 1.0f);

    // Enable z-buffer test
    glEnable(GL_DEPTH_TEST);

    // Seed the PRNG with the current time for any random elements in the world
    std::srand(std::time(NULL));
}

int main(int argc, char **argv) {
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

    // Initialize the AudioManager and get its instance
    AudioManager& audioManager = AudioManager::instance();

    // Initialize the GameManager and get its instance
    GameManager& gameManager = GameManager::instance();

    // Instantiate the current game world and player then load the level.
    GameWorld world;
    gameManager.setGameWorld(&world);
    std::shared_ptr<GameObject> player;

    LevelLoader& levelLoader = LevelLoader::instance();
    if (levelLoader.loadLevel(world, player)) {
        std::cerr << "Error loading level." << std::endl;
        return EXIT_FAILURE;
    }

	 // The current game camera
	 Camera camera(player);

	// Set the manager to the current camera
    gameManager.setCamera(&camera);

    // Set the manager to the current player object
    gameManager.setPlayer(player);

    // Set the current view frustum
    gameManager.setViewFrustum(new ViewFrustum());

    // Set the Shadow Map Object
    ShadowMap* shadowMap = new ShadowMap();
    gameManager.setShadowMap(shadowMap);

    // Create menu
    Menu* menu = new Menu();
    menu->init();
    gameManager.setMenu(menu);

    // Add all static objects before this!!!
    world.init();

    gameManager.setTime(15.0);
	gameManager.setGlobalColorScale(1.0f);

    // Loop until the user closes the window
    int numFramesInSecond = 0;
    constexpr double dt = 1.0 / 60.0;
    double totalTime = 0.0;
    double secondClock = 0.0;
    double startTime = glfwGetTime();
    double previousTime = startTime;

    audioManager.startSoundtrack();

    while (!windowManager.isClosed()) {

        // Poll for and process events
        windowManager.pollEvents();

        if(!gameManager.getMenu()->isActive()) {
            double currentTime = glfwGetTime();
            double elapsedTime = currentTime - previousTime;
            if(gameManager.getMenu()->leftMenuThisFrame_) {
                gameManager.getMenu()->leftMenuThisFrame_ = false;
                elapsedTime -= gameManager.getMenu()->menuTime_;
            }
            previousTime = currentTime;

            while (elapsedTime > 0.0) {
                double deltaTime = std::min(elapsedTime, dt);

                // Update all game objects
                world.updateGameObjects(deltaTime, totalTime);
                camera.update(deltaTime);

                gameManager.decreaseTime(deltaTime);

                if (gameManager.gameOver_) {
                    gameManager.showScore();
                    //return EXIT_SUCCESS;
                }

                elapsedTime -= deltaTime;
                totalTime += deltaTime;
                secondClock += deltaTime;
            }
        } else {
            world.updateMenu();
        }
        // Update the window in-case of resizing, etc.
        windowManager.update();

        // Render all objects to shadow map
        world.renderShadowMap();

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

        // All audio updating occurs here.
        audioManager.update();
    }

    return EXIT_SUCCESS;
}
