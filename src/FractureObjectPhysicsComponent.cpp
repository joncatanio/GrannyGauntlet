#include "FractureObjectPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "AudioManager.h"
#include "ParticleSystem.h"
#include <glm/gtx/rotate_vector.hpp>

FractureObjectPhysicsComponent::FractureObjectPhysicsComponent() {}

FractureObjectPhysicsComponent::~FractureObjectPhysicsComponent() {}

void FractureObjectPhysicsComponent::initObjectPhysics() {
	gravity = 10.0f;
	yVelocity = 0.0f;
}

void FractureObjectPhysicsComponent::updatePhysics(float deltaTime) {
	GameWorld& world = GameManager::instance().getGameWorld();

	glm::vec3 oldPosition = holder_->getPosition();
	glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity *
		holder_->direction * deltaTime);

	newPosition += glm::vec3(0.0, yVelocity * deltaTime, 0.0);
	holder_->setPosition(newPosition);
	updateBoundingBox();

	if (holder_->fracture && holder_->getPosition().y > 0.0f) {
		yVelocity -= gravity * deltaTime;
	}
	else if (holder_->velocity != 0.0) {
		yVelocity = 0.0f;
		holder_->velocity = 0.0f;

		// Set the position above ground plane.
		// TODO (noj) we'll want to set the position relative to the object it lands on.
		glm::vec3 newPos = holder_->getPosition();
		newPos.y = 1.0f;
		holder_->setPosition(newPos);
	}

	// All fracturing updating happens here
	if (holder_->fracture) {
		if (holder_->velocity == 0.0f) {
			world.rmDynamicGameObject(holder_);
		}

		std::shared_ptr<std::vector<glm::vec3>> fractureDirections = holder_->getFragmentDirs();
		std::shared_ptr<std::vector<glm::vec3>> fracturePositions = holder_->getFragmentPos();
		int numFrags = fracturePositions->size();

		for (int i = 0; i < numFrags; i++) {
			glm::vec3 newFractPos = fracturePositions->at(i) + (holder_->velocity *
				fractureDirections->at(i) * deltaTime);

			newFractPos += glm::vec3(0.0, yVelocity * deltaTime, 0.0);
			fracturePositions->at(i) = newFractPos;
		}
	}
	else {
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

				// Initialize animation parameters.
				animated = true;
				animRotAxis = rotAxis;

				// Play sound effect.
				AudioManager& audioManager = AudioManager::instance();
				audioManager.playEffect("FireHydrant Clank");

				// Initialize fracture variables if the player hits the object hard enough
				if (objHit->velocity >= 10) {
					// Do effects once on hit
					if (!holder_->fracture) {
						// Spawn billboard
						holder_->spawnPlayerHitBillboardEffect(holder_->getPosition());
					}

					holder_->fracture = true;
					holder_->setFragmentDirs(holder_->getRenderComponent()->getShape()->calcFragmentDir(reactDir));
				}

				// Initialize the power-up
				objHit->initPowerup("speed", 3.0);

			}
			else if (objTypeHit == GameObjectType::STATIC_OBJECT ||
				objTypeHit == GameObjectType::DYNAMIC_OBJECT) {
				std::shared_ptr<BoundingBox> objBB = objHit->getBoundingBox();
				std::shared_ptr<BoundingBox> thisBB = holder_->getBoundingBox();

				glm::vec3 normal = objBB->calcReflNormal(thisBB, 1.5f);
				holder_->direction = glm::reflect(holder_->direction, normal);
				animRotAxis = glm::cross(holder_->direction, glm::vec3(0, 1, 0));

				newPosition = oldPosition + (holder_->velocity * holder_->direction * deltaTime);
				newPosition += glm::vec3(0.0, yVelocity* deltaTime, 0.0);
				holder_->setPosition(newPosition);
				updateBoundingBox();
			}
		}
	}

	if (animated) {
		updateAnimation(deltaTime);
	}
}

void FractureObjectPhysicsComponent::updateAnimation(float deltaTime) {
	if (holder_->velocity == 0.0f) {
		animated = false;
	}
	else {
		holder_->addRotation(deltaTime * animSpeed, animRotAxis);
	}
}
