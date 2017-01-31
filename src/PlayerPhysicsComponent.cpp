#include "PlayerPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

PlayerPhysicsComponent::PlayerPhysicsComponent() {}

PlayerPhysicsComponent::~PlayerPhysicsComponent() {}

void PlayerPhysicsComponent::initObjectPhysics() {
   updateBoundingBox();
}

void PlayerPhysicsComponent::updateBoundingBox() {
	BoundingBox& boundBox = holder_->boundBox;
	MatrixTransform transform = holder_->transform;

	boundBox.update(transform.getTransform());
}

void PlayerPhysicsComponent::updatePhysics(float deltaTime) {
   if (holder_->toggleMovement) {
      glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity *
         glm::normalize(holder_->direction) * deltaTime);
      holder_->setPosition(newPosition);
      updateBoundingBox();
   }
}
