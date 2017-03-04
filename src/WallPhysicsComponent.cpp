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
    originalScale = holder_->getScale();
}

void WallPhysicsComponent::updateAnimation(float deltaTime) {
    animTime += deltaTime;
    if(animTime >= 2.0) {
        holder_->setScale(originalScale);
        animated = false;
    } else {
        float animSin = fabs(sin(animTime * M_PI));
        float xzScale = 1.0 - animSin / 2.0;
        float yScale = 1.0 + animSin;
        glm::vec3 animScale = glm::vec3(originalScale.x * xzScale, originalScale.y * yScale, originalScale.z * xzScale);
        holder_->setScale(animScale);
    }
}