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

   if (holder_->toggleMovement && holder_->velocity != 0.0f) {
      glm::vec3 oldPosition = holder_->getPosition();
      glm::vec3 newPosition = oldPosition + (holder_->velocity *
         glm::normalize(holder_->direction) * deltaTime);
      holder_->setPosition(newPosition);
      updateBoundingBox();

      // Check player collision against static objects
      GameWorld& world = GameManager::instance().getGameWorld();
      std::shared_ptr<GameObject> objHit = world.checkCollision(holder_);
      if (objHit != nullptr) {
         GameObjectType objTypeHit = objHit->type;

         if (objTypeHit == GameObjectType::STATIC_OBJECT) {
            BoundingBox* objHitBB = objHit->getBoundingBox();
            glm::vec3 normalOfObjHit = objHitBB->calcReflNormal(getBoundingBox());

            if (normalOfObjHit.x != 0.0f) {
               newPosition.x = oldPosition.x;
            }

            if (normalOfObjHit.z != 0.0f) {
               newPosition.z = oldPosition.z;
            }

            holder_->setPosition(newPosition);
            updateBoundingBox();
         }
         if (objTypeHit == GameObjectType::FINISH_OBJECT) {
            GameManager::instance().gameOver_ = true;
         }
      }
   }









   /*if (holder_->toggleMovement) {
      glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity *
         glm::normalize(holder_->direction) * deltaTime);
      holder_->setPosition(newPosition);
      updateBoundingBox();
   }

   // Check player collision against static objects
   GameWorld& world = GameManager::instance().getGameWorld();
   std::shared_ptr<GameObject> objHit = world.checkCollision(holder_);
   if (objHit != nullptr) {
      GameObjectType objTypeHit = objHit->type;

      if (objTypeHit == GameObjectType::STATIC_OBJECT) {
         if (holder_->velocity != 0.0f) {
            glm::vec3 newPosition = holder_->getPosition() - (holder_->velocity * 
               glm::normalize(holder_->direction) * deltaTime);
            holder_->setPosition(newPosition);
            updateBoundingBox();
         }
      }
      if (objTypeHit == GameObjectType::FINISH_OBJECT) {
         GameManager::instance().gameOver_ = true;
      }
   }*/
}
