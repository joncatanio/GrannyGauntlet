#include "PlayerPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

PlayerPhysicsComponent::PlayerPhysicsComponent() {}

PlayerPhysicsComponent::~PlayerPhysicsComponent() {}

void PlayerPhysicsComponent::initObjectPhysics() {
   updateBoundingBox();
}

void PlayerPhysicsComponent::updatePhysics(float deltaTime) {
   if (holder_->toggleMovement) {
      glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity *
         glm::normalize(holder_->direction) * deltaTime);
      holder_->setPosition(newPosition);
      updateBoundingBox();
   }

   // Check player collision against static objects
   GameWorld& world = GameManager::instance().getGameWorld();
   GameObjectType objTypeHit = world.checkCollision(holder_)->type;

   if (objTypeHit == GameObjectType::STATIC_OBJECT) {
      if (holder_->velocity != 0.0f) {
         glm::vec3 newPosition = holder_->getPosition() - (holder_->velocity * 
            glm::normalize(holder_->direction) * deltaTime);
         holder_->setPosition(newPosition);
         updateBoundingBox();
      }
   }
}
