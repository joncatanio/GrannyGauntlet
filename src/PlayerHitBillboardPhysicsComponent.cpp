#include "PlayerHitBillboardPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

void PlayerHitBillboardPhysicsComponent::updatePhysics(float deltaTime) {
	currentLifeTime += deltaTime;

	if (currentLifeTime > maxLifeTime) {
		GameManager& gameManager = GameManager::instance();
		GameWorld& world = gameManager.getGameWorld();

		world.rmDynamicGameObject(holder_);
	}
	else {
		glm::vec3 newScale = glm::vec3(0.25f) + glm::vec3(sin(currentLifeTime * M_PI));
		holder_->setScale(newScale);
	}
}
