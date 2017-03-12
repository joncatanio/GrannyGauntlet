#include "PlayerPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "PlayerBoundingSphere.h"

PlayerPhysicsComponent::PlayerPhysicsComponent() {}

PlayerPhysicsComponent::~PlayerPhysicsComponent() {}

void PlayerPhysicsComponent::initBoundingBox(glm::vec3& minBoundPt, glm::vec3& maxBoundPt) {
	std::cout << "HERE@@" << std::endl;

#ifdef USE_PLAYER_BOUNDING_SPHERE
	boundBox_ = std::make_shared<PlayerBoundingSphere>(PlayerBoundingSphere(minBoundPt, maxBoundPt));
#else
	boundBox_ = std::make_shared<BoundingBox>(BoundingBox(minBoundPt, maxBoundPt));
#endif

	updateBoundingBox();
}

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
      std::vector<std::shared_ptr<GameObject>> objsHit = world.checkCollision(holder_);

      for (auto objHit : objsHit) {
         GameObjectType objTypeHit = objHit->type;

         if (objTypeHit == GameObjectType::STATIC_OBJECT) {
			 std::shared_ptr<BoundingBox> objHitBB = objHit->getBoundingBox();
            glm::vec3 normalOfObjHit = objHitBB->calcReflNormal(getBoundingBox(), 0.1f);

            if (normalOfObjHit.x != 0.0f) {
               newPosition.x = oldPosition.x;
            }

            if (normalOfObjHit.y != 0.0f) {
               newPosition.y = oldPosition.y;
            }

            if (normalOfObjHit.z != 0.0f) {
               newPosition.z = oldPosition.z;
            }

			/*if (normalOfObjHit.x != 0.0f && normalOfObjHit.z != 0.0f) {
				newPosition = oldPosition + glm::normalize(glm::vec3(normalOfObjHit.x, 0.0f, normalOfObjHit.z)) * glm::vec3(5.0);
			}*/
         }
         if (objTypeHit == GameObjectType::FINISH_OBJECT) {
            GameManager::instance().gameOver_ = true;
         }
      }

      if (!objsHit.empty()) {
         holder_->setPosition(newPosition);
         updateBoundingBox();
      }
   }
}
