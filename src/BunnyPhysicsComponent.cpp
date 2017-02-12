#include "BunnyPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "MaterialManager.h"

BunnyPhysicsComponent::BunnyPhysicsComponent() {

}

BunnyPhysicsComponent::~BunnyPhysicsComponent() {

}

void BunnyPhysicsComponent::initObjectPhysics() {
	GameWorld& world = GameManager::instance().getGameWorld();

	setupInitialRotation();
	updateBoundingBox();

	GameObjectType objTypeHit = world.checkCollision(holder_)->type;

	// Don't randomly place a bunny on another bunny
	// Bunnies with no direction (static at 0, 0, 0) are currently OK - rare and adds an interesting twist
	while (objTypeHit != GameObjectType::NO_OBJECT) {

		// Get a random start location between (-10, 0, -10) and (10, 0, 10)
		float randomStartX = (std::rand() % 20) - 10.0f;
		float randomStartZ = (std::rand() % 20) - 10.0f;

		glm::vec3 startPosition(randomStartX, 1.0f, randomStartZ);
		glm::vec3 startDirection(randomStartX, 0.0f, randomStartZ);

		holder_->setPosition(startPosition);
		holder_->direction = glm::normalize(startDirection);

		// Rotate early in-case it affects bounding box collision
		setupInitialRotation();
		updateBoundingBox();

		objTypeHit = world.checkCollision(holder_)->type;
	}
}

void BunnyPhysicsComponent::setupInitialRotation() {
	float cosOfDir = glm::dot(glm::vec3(-1.0f, 0.0f, 0.0f), holder_->direction);

	float rotationAngle = glm::acos(cosOfDir);
	rotationAngle = holder_->direction.z < 0 ? -rotationAngle : rotationAngle;

	holder_->setYAxisRotation(rotationAngle);
}

void BunnyPhysicsComponent::updatePhysics(float deltaTime) {
	GameWorld& world = GameManager::instance().getGameWorld();

	glm::vec3 oldPosition = holder_->getPosition();

	// Update position of GameObject (if a collision occurs, will reset to old position and update position again)
	glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity * holder_->direction * deltaTime);
	holder_->setPosition(newPosition);
	updateBoundingBox();

	// If we hit someone or we're at the edge of the acceptable "world", then reverse direction
	GameObjectType objTypeHit = world.checkCollision(holder_)->type;
	
	if (objTypeHit == GameObjectType::PLAYER) {
		if (holder_->velocity != 0.0f) {
			// Only update bunny on hit once
			holder_->velocity = 0.0f;
			holder_->changeMaterial(MaterialManager::instance().getMaterial("Jade"));
			world.registerBunnyHit();
		}
	} else if (objTypeHit == GameObjectType::STATIC_OBJECT || objTypeHit == GameObjectType::DYNAMIC_OBJECT) {
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
