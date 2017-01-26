#include "GameWorld.h"
#include "GameManager.h"

GameWorld::GameWorld()
	: updateCount(0),
	renderCount(0),
	numBunniesHit(0) {

	// Seed the PRNG with the current time for any random elements in the world
	std::srand(std::time(NULL));
}

GameWorld::~GameWorld() {}

void GameWorld::addNonStaticGameObject(GameObject* obj) {
	this->nonStaticGameObjects_.push_back(obj);
}

void GameWorld::addStaticGameObject(GameObject* obj) {
	this->staticGameObjects_.push_back(obj);
}

int GameWorld::getNumNonStaticGameObjects() {
	return this->nonStaticGameObjects_.size();
}

int GameWorld::getNumStaticGameObjects() {
	return this->staticGameObjects_.size();
}

void GameWorld::clearGameObjects() {
	this->nonStaticGameObjects_.clear();
}

void GameWorld::resetWorld() {
	this->clearGameObjects();
	this->numBunniesHit = 0;
}

void GameWorld::updateGameObjects(double deltaTime, double totalTime) {
	// Keep track of the last spawn time internally to know when to spawn next
	static double previousSpawnTime = 0.0;

	// Spawn a new bunny every ~3 seconds, and max out at 30 bunnies
	if (totalTime >= previousSpawnTime + 3.0 && getNumNonStaticGameObjects() < 30) {
		addBunnyToGameWorld();
		previousSpawnTime = totalTime;
	}

	for each (GameObject* obj in this->nonStaticGameObjects_) {
		obj->update(deltaTime);
	}

	updateCount++;
}

void GameWorld::drawGameObjects() {
	GameManager& gameManager = GameManager::instance();
	Camera& camera = gameManager.getCamera();

	// TODO(rgarmsen): Move these two into some manager class from main
	// Update the frame buffer in-case of resizing, etc.
	//updateFrameBuffer();

	// Get the current aspect ratio
	//aspect = getAspectRatio();

	// Create the matrix stacks
	std::shared_ptr<MatrixStack> P = std::make_shared<MatrixStack>();
	std::shared_ptr<MatrixStack> M = std::make_shared<MatrixStack>();
	std::shared_ptr<MatrixStack> V = std::make_shared<MatrixStack>();

	// Apply perspective projection
	P->pushMatrix();
	P->perspective(45.0f, aspect, 0.01f, 100.0f);

	// Set up view Matrix
	V->pushMatrix();
	V->loadIdentity();
	V->lookAt(camera.getEye(), camera.getTarget(), camera.getUp());

	// Draw non-static objects
	for each (GameObject* obj in this->nonStaticGameObjects_) {
		obj->draw(P, M, V);
	}

	// Draw static objects
	for each (GameObject *obj in this->staticGameObjects_) {
		obj->draw(P, M, V);
	}

	renderCount++;
}

GameObjectType GameWorld::checkCollision(GameObject* objToCheck) {
	GameManager& gameManager = GameManager::instance();
	Camera& camera = gameManager.getCamera();

	// Check the camera against the object
	if (objToCheck->boundBox.checkIntersection(camera.boundBox)) {
		return GameObjectType::CAMERA;
	}

	// Check against static objects
	for each(GameObject* obj in staticGameObjects_) {
		if (objToCheck->boundBox.checkIntersection(obj->boundBox)) {
			return obj->type;
		}
	}

	// Check against other non-static objects
	for each(GameObject* obj in nonStaticGameObjects_) {
		if (obj != objToCheck && objToCheck->boundBox.checkIntersection(obj->boundBox)) {
			return obj->type;
		}
	}

	return GameObjectType::NO_OBJECT;
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

	Eigen::Vector3f startPosition(randomStartX, 1.0f, randomStartZ);

	Eigen::Vector3f startDirection(randomStartX, 0.0f, randomStartZ);
	startDirection.normalize();

	float startVelocity = 5.0f;

	Eigen::Vector3f initialScale(1.0f, 1.0f, 1.0f);

	BunnyPhysicsComponent* bunnyPhysicsComp = new BunnyPhysicsComponent();
	BunnyRenderComponent* bunnyRenderComp = new BunnyRenderComponent(bunnyShape, progPhong, brass);

	GameObject* bunnyObj = new GameObject(
		GameObjectType::NONSTATIC_OBJECT, 
		startPosition, 
		startDirection, 
		startVelocity, 
		initialScale,
		NULL, 
		bunnyPhysicsComp, 
		bunnyRenderComp);

	this->addNonStaticGameObject(bunnyObj);
}
