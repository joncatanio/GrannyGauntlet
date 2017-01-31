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

void GameWorld::addDynamicGameObject(GameObject* obj) {
	this->dynamicGameObjects_.push_back(obj);
}

void GameWorld::addStaticGameObject(GameObject* obj) {
	this->staticGameObjects_.push_back(obj);
}

int GameWorld::getNumDynamicGameObjects() {
	return this->dynamicGameObjects_.size();
}

int GameWorld::getNumStaticGameObjects() {
	return this->staticGameObjects_.size();
}

void GameWorld::clearGameObjects() {
	this->dynamicGameObjects_.clear();
}

void GameWorld::resetWorld() {
	this->clearGameObjects();
	this->numBunniesHit = 0;
}

void GameWorld::updateGameObjects(double deltaTime, double totalTime) {
	// Keep track of the last spawn time internally to know when to spawn next
	static double previousSpawnTime = 0.0;
    static double previousCookieTime = 0.0;

	// Spawn a new bunny every ~3 seconds, and max out at 30 bunnies
	if (totalTime >= previousSpawnTime + 3.0 && getNumDynamicGameObjects() < 30) {
		addBunnyToGameWorld();
		previousSpawnTime = totalTime;
	}

    //TODO(nurgan) use Input Handler, when it is implemented
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if(totalTime >= previousCookieTime + 0.5) {
            throwCookie();
            previousCookieTime = totalTime;
        }
    }

	for (GameObject* obj : this->dynamicGameObjects_) {
		obj->update(deltaTime);
	}

	updateCount++;
}

void GameWorld::drawGameObjects() {
	GameManager& gameManager = GameManager::instance();
	Camera& camera = gameManager.getCamera();
	
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
	for (GameObject* obj : this->dynamicGameObjects_) {
		obj->draw(P, M, V);
	}

	// Draw static objects
	for (GameObject *obj : this->staticGameObjects_) {
		obj->draw(P, M, V);
	}

	renderCount++;
}

GameObjectType GameWorld::checkCollision(GameObject* objToCheck) {
	GameManager& gameManager = GameManager::instance();
   GameObject& player = gameManager.getPlayer();

	// Check the player against the object
	if (objToCheck->boundBox.checkIntersection(player.boundBox)) {
		return GameObjectType::PLAYER;
	}

	// Check against static objects
	for (GameObject* obj : staticGameObjects_) {
		if (objToCheck->boundBox.checkIntersection(obj->boundBox)) {
			return obj->type;
		}
	}

	// Check against other non-static objects
	for (GameObject* obj : dynamicGameObjects_) {
		if (obj != objToCheck && objToCheck->boundBox.checkIntersection(obj->boundBox)) {
			return obj->type;
		}
	}

	return GameObjectType::NO_OBJECT;
}

unsigned long GameWorld::getRenderCount() {
	return renderCount;
}

void GameWorld::throwCookie() {
	ShaderManager& shaderManager = ShaderManager::instance();
	std::shared_ptr<Program> progPhong = shaderManager.getShaderProgram("Phong");

	GameManager& gameManager = GameManager::instance();
	Camera& camera = gameManager.getCamera();
	static std::shared_ptr<Shape> cookieShape = std::make_shared<Shape>();
	static bool hasCookieLoaded = false;

	if (!hasCookieLoaded) {
		cookieShape->loadMesh(RESOURCE_DIR + "sphere.obj");
		cookieShape->resize();
		cookieShape->init();

		hasCookieLoaded = true;
	}

	float startVelocity = 20.0f;

	glm::vec3 initialScale(0.5f, 0.5f, 0.5f);

    CookiePhysicsComponent* cookiePhysicsComp = new CookiePhysicsComponent();
	BunnyRenderComponent* renderComp = new BunnyRenderComponent(cookieShape, progPhong, obsidian);

	GameObject* cookieObj = new GameObject(
			GameObjectType::DYNAMIC_OBJECT,
			camera.getEye(),
			camera.getLookAt(),
			startVelocity,
			initialScale,
			NULL,
            cookiePhysicsComp,
			renderComp);

	this->addDynamicGameObject(cookieObj);
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
	BunnyRenderComponent* bunnyRenderComp = new BunnyRenderComponent(bunnyShape, progPhong, brass);

	GameObject* bunnyObj = new GameObject(
		GameObjectType::DYNAMIC_OBJECT, 
		startPosition, 
		startDirection, 
		startVelocity, 
		initialScale,
		NULL, 
		bunnyPhysicsComp, 
		bunnyRenderComp);

	this->addDynamicGameObject(bunnyObj);
}
