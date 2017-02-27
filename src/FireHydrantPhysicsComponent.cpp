#include "FireHydrantPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include <glm/gtx/rotate_vector.hpp>

FireHydrantPhysicsComponent::FireHydrantPhysicsComponent() {}

FireHydrantPhysicsComponent::~FireHydrantPhysicsComponent() {}

void FireHydrantPhysicsComponent::initObjectPhysics() {
   gravity = 10.0f;
   yVelocity = 0.0f;
}

void FireHydrantPhysicsComponent::updatePhysics(float deltaTime) {
   GameWorld& world = GameManager::instance().getGameWorld();

   glm::vec3 oldPosition = holder_->getPosition();
   glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity *
      holder_->direction * deltaTime);

   newPosition += glm::vec3(0.0, yVelocity * deltaTime, 0.0);
   holder_->setPosition(newPosition);
   updateBoundingBox();

   if (holder_->getPosition().y > 1.0f) {
      yVelocity -= gravity * deltaTime;
   } else {
      yVelocity = 0.0f;
      holder_->velocity = 0.0f;
   }

   std::vector<std::shared_ptr<GameObject>> objsHit = world.checkCollision(holder_);
   if (!objsHit.empty()) {
      std::shared_ptr<GameObject> objHit = objsHit[0];
      GameObjectType objTypeHit = objHit->type;
      
      if (objTypeHit == GameObjectType::PLAYER) {
         glm::vec3 reactDir = objHit->direction;
         glm::vec3 rotAxis = glm::cross(objHit->direction, glm::vec3(0, 1, 0));
         // Give the direction of the hydrant a vertical component.
         reactDir = glm::rotate(reactDir, (float)(M_PI / 6.0f), rotAxis);

         holder_->direction = reactDir;
         holder_->velocity = objHit->velocity * 2.0;
      } else if (objTypeHit == GameObjectType::STATIC_OBJECT ||
                 objTypeHit == GameObjectType::DYNAMIC_OBJECT) {
         BoundingBox* objBB = objHit->getBoundingBox();
         BoundingBox* thisBB = holder_->getBoundingBox();

         glm::vec3 normal = objBB->calcReflNormal(*thisBB);
         holder_->direction = glm::reflect(holder_->direction, normal);

         newPosition = oldPosition + (holder_->velocity * holder_->direction * deltaTime);
         newPosition += glm::vec3(0.0, yVelocity* deltaTime, 0.0);
         holder_->setPosition(newPosition);
         updateBoundingBox();
      }
   }
}
