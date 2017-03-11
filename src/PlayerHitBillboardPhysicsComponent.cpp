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
		holder_->setScale(glm::vec3(1.0) + glm::vec3(sin(currentLifeTime * M_PI)));
	}
}
