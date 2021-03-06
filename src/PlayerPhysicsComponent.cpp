#include "PlayerPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "PlayerBoundingSphere.h"

#include <iostream>

PlayerPhysicsComponent::PlayerPhysicsComponent() {}

PlayerPhysicsComponent::~PlayerPhysicsComponent() {}

void PlayerPhysicsComponent::initBoundingBox(glm::vec3& minBoundPt, glm::vec3& maxBoundPt) {
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
	GameManager& gameManager = GameManager::instance();
	float colorScale = gameManager.getGlobalColorScale();
	colorScale = std::max(0.0f, colorScale - (deltaTime / 5.0f));
	gameManager.setGlobalColorScale(colorScale);

	if (!holder_->canMove) {

		// Don't want player's time to go down while frozen
		gameManager.increaseTime(deltaTime);

		if (colorScale == 0.0f) {

			// If the world is all greyed out, allow the player to start!
			holder_->canMove = true;
		}
	}

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

         if (objTypeHit == GameObjectType::STATIC_OBJECT || objTypeHit == GameObjectType::FINISH_OBJECT) {
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
         }
         if (objTypeHit == GameObjectType::FINISH_OBJECT) {
			 GameManager& gameManager = GameManager::instance();
			gameManager.gameOver_ = true;

			if (gameManager.getScore() < 0) {
				// If the player has a negative score, don't give a time bonus
				gameManager.setTime(1.0f);
			}
         }
      }

      if (!objsHit.empty()) {
         holder_->setPosition(newPosition);
         updateBoundingBox();
      }
   }
}
