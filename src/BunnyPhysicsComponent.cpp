#include "BunnyPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

BunnyPhysicsComponent::BunnyPhysicsComponent() {

}

BunnyPhysicsComponent::~BunnyPhysicsComponent() {

}

void BunnyPhysicsComponent::initObjectPhysics() {
	GameWorld& world = GameManager::instance().getGameWorld();
	GameObjectType objTypeHit = world.checkCollision(holder_);

	// Don't randomly place a bunny on another bunny
	// Bunnies with no direction (static at 0, 0, 0) are currently OK - rare and adds an interesting twist
	while (objTypeHit != GameObjectType::NO_OBJECT) {

		// Get a random start location between (-10, 0, -10) and (10, 0, 10)
		float randomStartX = (std::rand() % 20) - 10.0f;
		float randomStartZ = (std::rand() % 20) - 10.0f;

		Eigen::Vector3f startPosition(randomStartX, 1.0f, randomStartZ);
		Eigen::Vector3f startDirection(randomStartX, 0.0f, randomStartZ);

		holder_->setPosition(startPosition);
		holder_->direction = startDirection.normalized();
		holder_->calculateAndSetInitialRotation();

		updateBoundingBox();

		objTypeHit = world.checkCollision(holder_);
	}
}

void BunnyPhysicsComponent::updateBoundingBox() {
	BoundingBox& boundBox = holder_->boundBox;
	MatrixTransform transform = holder_->transform;

	boundBox.update(transform.getTransform());
}

void BunnyPhysicsComponent::updatePhysics(double deltaTime) {
	GameWorld& world = GameManager::instance().getGameWorld();

	Eigen::Vector3f oldPosition = holder_->getPosition();

	// Update position of GameObject (if a collision occurs, will reset to old position and update position again)
	Eigen::Vector3f newPosition = holder_->getPosition() + (holder_->velocity * holder_->direction * deltaTime);
	holder_->setPosition(newPosition);
	updateBoundingBox();

	// If we hit someone or we're at the edge of the acceptable "world", then reverse direction
	GameObjectType objTypeHit = world.checkCollision(holder_);
	
	if (objTypeHit == GameObjectType::CAMERA) {
		if (holder_->velocity != 0.0f) {
			// Only update bunny on hit once
			holder_->velocity = 0.0f;
			holder_->changeMaterial(jade);
			world.registerBunnyHit();
		}
	} else if (objTypeHit == GameObjectType::STATIC_OBJECT || objTypeHit == GameObjectType::NONSTATIC_OBJECT) {
		if (holder_->velocity != 0.0f) {
			// Only update bunny on hit if moving
			holder_->direction = -holder_->direction;
			holder_->setYAxisRotation(180.0f + holder_->getYAxisRotation());

			// Update position of GameObject
			newPosition = oldPosition + (holder_->velocity * holder_->direction * deltaTime);
			holder_->setPosition(newPosition);
			updateBoundingBox();
		}
	}
}
