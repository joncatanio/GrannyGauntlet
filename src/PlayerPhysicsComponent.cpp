#include "PlayerPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

PlayerPhysicsComponent::PlayerPhysicsComponent() {}

PlayerPhysicsComponent::~PlayerPhysicsComponent() {}

void PlayerPhysicsComponent::initObjectPhysics() {

}

void PlayerPhysicsComponent::updateBoundingBox() {

}

void PlayerPhysicsComponent::updatePhysics(float deltaTime) {
   if (holder_->toggleXMovement || holder_->toggleZMovement) {
      glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity *
         glm::normalize(holder_->direction) * deltaTime);
      holder_->setPosition(newPosition);
   }
   // TODO (noj) update bounding box here.
}
