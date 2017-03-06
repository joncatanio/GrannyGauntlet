#include "BillboardPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "MaterialManager.h"

BillboardPhysicsComponent::BillboardPhysicsComponent() {

}

BillboardPhysicsComponent::~BillboardPhysicsComponent() {

}

void BillboardPhysicsComponent::initObjectPhysics() {

}

void BillboardPhysicsComponent::updatePhysics(float deltaTime) {
	GameWorld& world = GameManager::instance().getGameWorld();

	glm::vec3 oldPosition = holder_->getPosition();

	// Update position of GameObject (if a collision occurs, will reset to old position and update position again)
	glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity * holder_->direction * deltaTime);
	holder_->setPosition(newPosition);
	updateBoundingBox();

	// If we hit someone or we're at the edge of the acceptable "world", then reverse direction
	std::vector<std::shared_ptr<GameObject>> objsHit = world.checkCollision(holder_);
	if (!objsHit.empty()) {
		std::shared_ptr<GameObject> objHit = objsHit[0];
		GameObjectType objTypeHit = objHit->type;

		if (objTypeHit == GameObjectType::PLAYER) {
			if (holder_->velocity != 0.0f) {
				// Only update bunny on hit once
				holder_->velocity = 0.0f;
				holder_->changeMaterial(MaterialManager::instance().getMaterial("Jade"));
				world.registerBunnyHit();
			}
		}
		else if (objTypeHit == GameObjectType::STATIC_OBJECT || objTypeHit == GameObjectType::DYNAMIC_OBJECT) {
			if (holder_->velocity != 0.0f) {
				// Only update bunny on hit if moving
				holder_->direction = -holder_->direction;
				holder_->setYAxisRotation(M_PI + holder_->getYAxisRotation());

				// Update position of GameObject
				newPosition = oldPosition + (holder_->velocity * holder_->direction * deltaTime);
				holder_->setPosition(newPosition);
				updateBoundingBox();
			}
		}
	}
}
