#include "CookieHitBillboardPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

void CookieHitBillboardPhysicsComponent::updatePhysics(float deltaTime) {
	currentLifeTime += deltaTime;

	if (currentLifeTime > maxLifeTime) {
		GameManager& gameManager = GameManager::instance();
		GameWorld& world = gameManager.getGameWorld();

		world.rmDynamicGameObject(holder_);
	}
	else {
		glm::vec3 oldPosition = holder_->getPosition();
		glm::vec3 newPosition = oldPosition + (holder_->velocity * holder_->direction * deltaTime);

		holder_->setPosition(newPosition);
		//updateBoundingBox();
	}
}
