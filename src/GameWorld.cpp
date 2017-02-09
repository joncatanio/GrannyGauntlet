#include <glm/gtx/rotate_vector.hpp>

#include <glm/gtx/rotate_vector.hpp>

#include "CookieActionComponent.h"
#include "GameManager.h"
#include "GameWorld.h"
#include "ShaderManager.h"
#include "WindowManager.h"

GameWorld::GameWorld()
	: updateCount(0),
	renderCount(0),
	numBunniesHit(0) {

	// Seed the PRNG with the current time for any random elements in the world
	std::srand(std::time(NULL));

	// TODO(rgarmsen2295): Make this look nicer
	addLight({ -10.0f, 10.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1 });
}

GameWorld::~GameWorld() {}

void GameWorld::addDynamicGameObject(GameObject* obj) {
	this->dynamicGameObjects_.push_back(obj);
}

void GameWorld::addStaticGameObject(GameObject* obj) {
	this->staticGameObjects_.push_back(obj);
}

void GameWorld::addLight(const Light& newLight) {
	lights.push_back(newLight);
}

int GameWorld::getNumDynamicGameObjects() {
	return this->dynamicGameObjects_.size();
}

int GameWorld::getNumStaticGameObjects() {
	return this->staticGameObjects_.size();
}

const std::vector<Light>& GameWorld::getLights() {
	return lights;
}

void GameWorld::clearGameObjects() {
	this->dynamicGameObjects_.clear();
}

void GameWorld::resetWorld() {
	this->clearGameObjects();
	this->numBunniesHit = 0;
}

void GameWorld::updateGameObjects(double deltaTime, double totalTime) {
	
#ifdef DEBUG_BUNNIES
	// Keep track of the last spawn time internally to know when to spawn next
	static double previousSpawnTime = 0.0;

	// Spawn a new bunny every ~3 seconds, and max out at 30 bunnies
	if (totalTime >= previousSpawnTime + 3.0 && getNumDynamicGameObjects() < 30) {
		addBunnyToGameWorld();
		previousSpawnTime = totalTime;
	}
#endif

	for (GameObject* obj : this->dynamicGameObjects_) {
		obj->update(deltaTime);
        obj->performAction(deltaTime, totalTime);
	}

	for (GameObject* obj : this->staticGameObjects_) {
		obj->update(deltaTime);
	}

	updateCount++;
}

void GameWorld::drawGameObjects() {
	GameManager& gameManager = GameManager::instance();
	Camera& camera = gameManager.getCamera();

	WindowManager& windowManager = WindowManager::instance();
	
	// Create the matrix stacks
	std::shared_ptr<MatrixStack> P = std::make_shared<MatrixStack>();
	std::shared_ptr<MatrixStack> M = std::make_shared<MatrixStack>();
	std::shared_ptr<MatrixStack> V = std::make_shared<MatrixStack>();

	// Apply perspective projection
	P->pushMatrix();
	P->perspective(45.0f, windowManager.getAspectRatio(), 0.01f, 200.0f);

	// Set up view Matrix
	V->pushMatrix();
	V->loadIdentity();
	V->lookAt(camera.getEye(), camera.getTarget(), camera.getUp());

	// Draw non-static objects
	for (GameObject* obj : this->dynamicGameObjects_) {
		obj->draw(P, M, V);
	}

	// Draw static objects
	for (GameObject *obj : this->staticGameObjects_) {
		obj->draw(P, M, V);
	}
	renderCount++;
}

GameObject* GameWorld::checkCollision(GameObject* objToCheck) {
	GameManager& gameManager = GameManager::instance();
   GameObject* player = &gameManager.getPlayer();


	// Check the player against the object
	if (player != objToCheck && objToCheck->checkIntersection(player)) {
		return player;
	}

	// Check against dynamic objects
	for (GameObject* obj : dynamicGameObjects_) {
		if (obj != objToCheck && objToCheck->checkIntersection(obj)) {
			return obj;
		}
	}

	// Check against static objects
	for (GameObject* obj : staticGameObjects_) {
		if (obj != objToCheck && objToCheck->checkIntersection(obj)) {
			return obj;
		}
	}

	return new GameObject(
            GameObjectType::NO_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(1.0),
            NULL,
            NULL,
            NULL,
            NULL);
}

unsigned long GameWorld::getRenderCount() {
	return renderCount;
}

// TODO(rgarmsen2295): Abstract into "bunny world" specific sub-class
int GameWorld::getNumBunniesHit() {
	return numBunniesHit;
}

// TODO(rgarmsen2295): Abstract into "bunny world" specific sub-class
void GameWorld::registerBunnyHit() {
	numBunniesHit++;
}

// TODO(rgarmsen2295): Abstract into "bunny world" specific sub-class
void GameWorld::addBunnyToGameWorld() {
	ShaderManager& shaderManager = ShaderManager::instance();
	std::shared_ptr<Program> progPhong = shaderManager.getShaderProgram("Phong");

	static std::shared_ptr<Shape> bunnyShape = std::make_shared<Shape>();
	static bool hasBunnyLoaded = false;

	// TODO(rgarmsen2295): Refactor into some sort of manager class to simply and remove complexity from GameWorld
	// Only perform bunny shape init stuff once
	if (!hasBunnyLoaded) {
		bunnyShape->loadMesh(RESOURCE_DIR + "bunny.obj");
		bunnyShape->resize();
		bunnyShape->init();

		hasBunnyLoaded = true;
	}

	// Get a random start location between (-10, 0, -10) and (10, 0, 10)
	float randomStartX = (std::rand() % 20) - 10.0f;
	float randomStartZ = (std::rand() % 20) - 10.0f;

	glm::vec3 startPosition(randomStartX, 1.0f, randomStartZ);

	glm::vec3 startDirection(randomStartX, 0.0f, randomStartZ);
	glm::normalize(startDirection);

	float startVelocity = 5.0f;

	glm::vec3 initialScale(1.0f, 1.0f, 1.0f);

	BunnyPhysicsComponent* bunnyPhysicsComp = new BunnyPhysicsComponent();
	BunnyRenderComponent* bunnyRenderComp = new BunnyRenderComponent(bunnyShape, "Phong", brass);

	GameObject* bunnyObj = new GameObject(
		GameObjectType::DYNAMIC_OBJECT, 
		startPosition, 
		startDirection, 
		startVelocity, 
		initialScale,
		NULL, 
		bunnyPhysicsComp,
		bunnyRenderComp,
        NULL);

	this->addDynamicGameObject(bunnyObj);
}
