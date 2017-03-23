#include <glm/gtx/rotate_vector.hpp>
#include <algorithm>
#include <memory>

#include "CookieActionComponent.h"
#include "GameManager.h"
#include "GameWorld.h"
#include "ViewFrustum.h"
#include "ShaderManager.h"
#include "ShapeManager.h"
#include "MaterialManager.h"
#include "WindowManager.h"
#include "SkyboxRenderComponent.h"

GameWorld::GameWorld()
	: updateCount(0),
	renderCount(0),
	numBunniesHit(0) {}

GameWorld::~GameWorld() {}

void GameWorld::addDynamicGameObject(std::shared_ptr<GameObject> obj) {
	dynamicGameObjectsToAdd_.push(obj);
}

void GameWorld::rmDynamicGameObject(std::shared_ptr<GameObject> obj) {
   dynamicGameObjectsToRemove_.push(obj);
}

void GameWorld::addStaticGameObject(std::shared_ptr<GameObject> obj) {
	staticGameObjectsToAdd_.push(obj);
}

void GameWorld::rmStaticGameObject(std::shared_ptr<GameObject> obj) {
   staticGameObjectsToRemove_.push(obj);
}

void GameWorld::setMenu(std::shared_ptr <GameObject> menu) {
    menu_ = menu;
}

void GameWorld::setSkybox(std::shared_ptr <GameObject> skybox) {
    skybox_ = skybox;
}

void GameWorld::setHellbox(std::shared_ptr <GameObject> hellbox) {
    hellbox_ = hellbox;
}

void GameWorld::addLight(const std::shared_ptr<Light> light) {
	switch(light->type) {
		case LightType::POINT:
			addPointLight(light);
			break;
		case LightType::AREA:
			addAreaLight(light);
			break;
		case LightType::DIRECTIONAL:
		default:
			addDirectionalLight(light);
			break;
	}
}

void GameWorld::addPointLight(const std::shared_ptr<Light> newLight) {
	pointLights.push_back(newLight);
}

void GameWorld::addDirectionalLight(const std::shared_ptr<Light> newLight) {
	directionalLights.push_back(newLight);
}

void GameWorld::addAreaLight(const std::shared_ptr<Light> newLight) {
	areaLights.push_back(newLight);
}

void GameWorld::addParticleSystem(std::shared_ptr <ParticleSystem> particleSystem) {
    particleSystemsToAdd_.push(particleSystem);
}
void GameWorld::rmParticleSystem(std::shared_ptr <ParticleSystem> particleSystem) {
    particleSystemsToRemove_.push(particleSystem);
}

int GameWorld::getNumDynamicGameObjects() {
	return dynamicGameObjects_.size();
}

int GameWorld::getNumStaticGameObjects() {
	return staticGameObjects_.size();
}

const std::vector<std::shared_ptr<Light>>& GameWorld::getPointLights() {
	return pointLights;
}

const std::vector<std::shared_ptr<Light>>& GameWorld::getDirectionalLights() {
	return directionalLights;
}

const std::vector<std::shared_ptr<Light>>& GameWorld::getAreaLights() {
	return areaLights;
}

void GameWorld::clearDynamicGameObjects() {
	dynamicGameObjects_.clear();
	while (!dynamicGameObjectsToAdd_.empty()) {
		dynamicGameObjectsToAdd_.pop();
	}
}

void GameWorld::clearStaticGameObjects() {
	staticGameObjects_.clear();
	while (!staticGameObjectsToAdd_.empty()) {
		staticGameObjectsToAdd_.pop();
	}
	staticGameObjectsTree_.clearTree();
}

void GameWorld::init() {
	updateInternalGameObjectLists();

	for (std::shared_ptr<GameObject> obj : staticGameObjects_) {
		staticGameObjectsTree_.addObject(obj);
	}

	staticGameObjectsTree_.buildTree();

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

	for (std::shared_ptr<GameObject> obj : dynamicGameObjects_) {
		obj->update(deltaTime);
        obj->performAction(deltaTime, totalTime);
	}

	for (std::shared_ptr<GameObject> obj : staticGameObjects_) {
		obj->update(deltaTime);
	}

    Camera& camera = GameManager::instance().getCamera();
    std::shared_ptr<MatrixStack> V = std::make_shared<MatrixStack>();
	V->pushMatrix();
	V->loadIdentity();
	V->lookAt(camera.getEye(), camera.getTarget(), camera.getUp());
    for (std::shared_ptr<ParticleSystem> ps : particleSystems_) {
        ps->update(totalTime, deltaTime, V);
    }

	updateInternalGameObjectLists();
	updateCount++;
}

void GameWorld::updateMenu() {
	menu_->update(0.0);
}

void GameWorld::drawGameObjects() {
	GameManager& gameManager = GameManager::instance();

    gameManager.getShadowMap()->bindForDraw();

	Camera& camera = gameManager.getCamera();
   ViewFrustum& viewFrustum = gameManager.getViewFrustum();

	WindowManager& windowManager = WindowManager::instance();


	// Create the matrix stacks
	std::shared_ptr<MatrixStack> P = std::make_shared<MatrixStack>();
	std::shared_ptr<MatrixStack> M = std::make_shared<MatrixStack>();
	std::shared_ptr<MatrixStack> V = std::make_shared<MatrixStack>();
    //create another projection, identical but with closer far plane for VFC
    std::shared_ptr<MatrixStack> cullP = std::make_shared<MatrixStack>();

	// Apply perspective projection
	P->pushMatrix();
    cullP->pushMatrix();
	//TODO(nurgan) check for better way of not clipping skybox instead of increasing far plane
	//P->perspective(45.0f, windowManager.getAspectRatio(), 0.01f, 300.0f);
    P->perspective(45.0f, windowManager.getAspectRatio(), GameManager::nearPlane, GameManager::camFarPlane);
    cullP->perspective(45.0f, windowManager.getAspectRatio(), GameManager::nearPlane, GameManager::cullFarPlane);

    // Set up view Matrix
	V->pushMatrix();
	V->loadIdentity();
	V->lookAt(camera.getEye(), camera.getTarget(), camera.getUp());

    // Calculate view frustum planes
    viewFrustum.extractPlanes(cullP->topMatrix(), V->topMatrix());

    // Draw dynamic objects
    for (std::shared_ptr<GameObject> obj : dynamicGameObjects_) {

        /* Every object needs to have a bounding box in order to cull.
        * If an object doesn't have a bounding box, cull it so we don't create
        * unseen problems with culling. */
        std::shared_ptr<BoundingBox> objBox = obj->getBoundingBox();
        if (objBox != nullptr) {
            if (!viewFrustum.cull(objBox)) {
                obj->draw(P, M, V);
            }
        }
        else {
            obj->draw(P, M, V);
        }
    }
    
    // Draw static objects
	staticGameObjectsTree_.cullAndDrawObjs(viewFrustum, true, P, M, V);

    // Draw skybox
    if(!gameManager.isInHellMode()) {
        skybox_->draw(P, M, V);
    } else {
        hellbox_->draw(P, M, V);
    }

    // Draw particles
    for (std::shared_ptr<ParticleSystem> ps : particleSystems_) {
        if (!viewFrustum.cull(ps)) {
            ps->draw(P, M, V);
        }
    }

	if(gameManager.getMenu()->isActive()) {
		glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		menu_->draw(P, M, V);
		glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
	}

	renderCount++;

   #ifdef DEBUG
   drawVFCViewport();
   #endif
}

void GameWorld::renderShadowMap() {

    GameManager::instance().getShadowMap()->bindForShadowPass();

    std::shared_ptr<MatrixStack> M = std::make_shared<MatrixStack>();

    // Draw non-static objects
    for (std::shared_ptr<GameObject> obj : dynamicGameObjects_) {
            obj->renderToShadowMap(M);
    }

    // Draw static objects
    for (std::shared_ptr<GameObject> obj : staticGameObjects_) {
            obj->renderToShadowMap(M);
    }
}

// For debugging view frustum culling. This is mostly magic.
void GameWorld::drawVFCViewport() {
	GameManager& gameManager = GameManager::instance();
	Camera& camera = gameManager.getCamera();
   ViewFrustum& viewFrustum = gameManager.getViewFrustum();
	WindowManager& windowManager = WindowManager::instance();
	
	// Create the matrix stacks
	std::shared_ptr<MatrixStack> P = std::make_shared<MatrixStack>();
	std::shared_ptr<MatrixStack> M = std::make_shared<MatrixStack>();
	std::shared_ptr<MatrixStack> V = std::make_shared<MatrixStack>();

   glClear(GL_DEPTH_BUFFER_BIT);
   glViewport(0, 0, (windowManager.getViewHeight() / 3.0),
      windowManager.getViewHeight() / 3.0);
   P->pushMatrix();
   P->ortho(-50.0f, 50.0f, -50.0f, 50.0f, 2.1f, 101.0f);
   V->pushMatrix();
   V->loadIdentity();
   V->lookAt(camera.getNoSpringEye() + glm::vec3(0, 100.0, 0), camera.getNoSpringEye(),
      glm::vec3(-1, 0, 0));

   // Draw dynamic objects
   for (std::shared_ptr<GameObject> obj : dynamicGameObjects_) {
		obj->draw(P, M, V);
   }

   // Draw static objects
   staticGameObjectsTree_.cullAndDrawObjs(viewFrustum, false, P, M, V);
}

std::vector<std::shared_ptr<GameObject>> GameWorld::checkCollision(std::shared_ptr<GameObject> objToCheck) {
	GameManager& gameManager = GameManager::instance();
	std::vector<std::shared_ptr<GameObject>> collidedObjs;

   std::shared_ptr<GameObject> player = gameManager.getPlayer();

	// Check the player against the object
	if (player != objToCheck && objToCheck->checkIntersection(player)) {
		collidedObjs.push_back(player);
	}

	// Check against dynamic objects
	for (std::shared_ptr<GameObject> obj : dynamicGameObjects_) {
		if (obj != objToCheck && objToCheck->checkIntersection(obj)) {
			collidedObjs.push_back(obj);
		}
	}

	// Check against static objects
	std::vector<std::shared_ptr<GameObject>> staticObjsCollided = staticGameObjectsTree_.checkIntersection(objToCheck);
	if (!staticObjsCollided.empty()) {
		collidedObjs.insert(collidedObjs.end(), 
		 std::make_move_iterator(staticObjsCollided.begin()),
		 std::make_move_iterator(staticObjsCollided.end()));
	}

	return collidedObjs;
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
   ShapeManager& shapeManager = ShapeManager::instance();
   MaterialManager& materialManager = MaterialManager::instance();

	// Get a random start location between (-10, 0, -10) and (10, 0, 10)
	float randomStartX = (std::rand() % 20) - 10.0f;
	float randomStartZ = (std::rand() % 20) - 10.0f;

	glm::vec3 startPosition(randomStartX, 1.0f, randomStartZ);

	glm::vec3 startDirection(randomStartX, 0.0f, randomStartZ);
	glm::normalize(startDirection);

	float startVelocity = 5.0f;

	glm::vec3 initialScale(1.0f, 1.0f, 1.0f);

	BunnyPhysicsComponent* bunnyPhysicsComp = new BunnyPhysicsComponent();
	BunnyRenderComponent* bunnyRenderComp = new BunnyRenderComponent(
		shapeManager.getShape("Bunny"), shaderManager.DefaultShader, materialManager.getMaterial("Brass"));

	std::shared_ptr<GameObject> bunnyObj = std::make_shared<GameObject>(
		GameObjectType::DYNAMIC_OBJECT, 
		startPosition, 
		startDirection, 
		startVelocity, 
		initialScale,
		nullptr, 
		bunnyPhysicsComp,
		bunnyRenderComp,
        nullptr);
	bunnyObj->initComponents();

	addDynamicGameObject(bunnyObj);
}

void GameWorld::updateInternalGameObjectLists() {
	while (!dynamicGameObjectsToAdd_.empty()) {
		dynamicGameObjects_.push_back(dynamicGameObjectsToAdd_.front());
		dynamicGameObjectsToAdd_.pop();
	}

	while (!staticGameObjectsToAdd_.empty()) {
		staticGameObjects_.push_back(staticGameObjectsToAdd_.front());
		staticGameObjectsToAdd_.pop();
	}

    while (!particleSystemsToAdd_.empty()) {
        particleSystems_.push_back(particleSystemsToAdd_.front());
        particleSystemsToAdd_.pop();
    }

   while (!dynamicGameObjectsToRemove_.empty()) {
      std::shared_ptr<GameObject> obj = dynamicGameObjectsToRemove_.front();
      dynamicGameObjects_.erase(std::remove(dynamicGameObjects_.begin(),
         dynamicGameObjects_.end(), obj), dynamicGameObjects_.end());

      dynamicGameObjectsToRemove_.pop();
   }

   while (!staticGameObjectsToRemove_.empty()) {
      std::shared_ptr<GameObject> obj = staticGameObjectsToRemove_.front();
      staticGameObjects_.erase(std::remove(staticGameObjects_.begin(),
         staticGameObjects_.end(), obj), staticGameObjects_.end());

      staticGameObjectsToRemove_.pop();
   }

    while (!particleSystemsToRemove_.empty()) {
        std::shared_ptr<ParticleSystem> obj = particleSystemsToRemove_.front();
        particleSystems_.erase(std::remove(particleSystems_.begin(),
                                             particleSystems_.end(), obj), particleSystems_.end());

        particleSystemsToRemove_.pop();
    }
}

void GameWorld::resetDeliverables() {
	for (std::shared_ptr<GameObject> obj : dynamicGameObjects_) {
		obj->resetDeliverable();
	}

	for (std::shared_ptr<GameObject> obj : staticGameObjects_) {
		obj->resetDeliverable();
	}
}