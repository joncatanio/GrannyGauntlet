#include "WallPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

#define _USE_MATH_DEFINES
#include <math.h>

WallPhysicsComponent::WallPhysicsComponent() {

}

WallPhysicsComponent::~WallPhysicsComponent() {

}

void WallPhysicsComponent::initObjectPhysics() {

}

void WallPhysicsComponent::updatePhysics(float deltaTime) {
    if(animated) {
        updateAnimation(deltaTime);
    }
}

void WallPhysicsComponent::startDeliveryAnimation() {
    animated = true;
    originalPosition = holder_->getPosition();
    velocity = startVelocity;
    numberOfJumps = 0;
}

void WallPhysicsComponent::updateAnimation(float deltaTime) {

    glm::vec3 currentPosition = holder_->getPosition();

    if (currentPosition.y < originalPosition.y - 0.00001) {
        numberOfJumps++;
        holder_->setPosition(originalPosition);
        velocity = startVelocity;
        if (numberOfJumps == maxJumps) {
            animated = false;
            return;
        }
    }

    glm::vec3 newPosition = glm::vec3(originalPosition.x, holder_->getPosition().y + deltaTime * velocity,
                                          originalPosition.z);

    holder_->setPosition(newPosition);
    velocity += deltaTime * gravity;

}