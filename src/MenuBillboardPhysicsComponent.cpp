#include "MenuBillboardPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

void MenuBillboardPhysicsComponent::updatePhysics(float deltaTime) {
	currentLifeTime += deltaTime;

	if (currentLifeTime > maxLifeTime) {
		GameManager& gameManager = GameManager::instance();
		GameWorld& world = gameManager.getGameWorld();

		world.rmDynamicGameObject(holder_);
	}
	else {

		GameManager& gameManager = GameManager::instance();

		glm::vec3 offset = gameManager.getCamera().getLookAt();
		//glm::vec3 newPosition = gameManager.getCamera().getEye() + glm::vec3(10.0f * offset.x, 0.0, 10.0f * offset.z);
		glm::vec3 newPosition = gameManager.getCamera().getEye() + 10.0f * offset;

		holder_->setPosition(newPosition);
	}
}
