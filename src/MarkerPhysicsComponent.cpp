#include "MarkerPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

#define _USE_MATH_DEFINES
#include <math.h>

MarkerPhysicsComponent::MarkerPhysicsComponent() {

}

MarkerPhysicsComponent::~MarkerPhysicsComponent() {

}

void MarkerPhysicsComponent::initObjectPhysics() {
   holder_->addRotation(90.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void MarkerPhysicsComponent::updatePhysics(float deltaTime) {
    updateAnimation(deltaTime);
}

void MarkerPhysicsComponent::updateAnimation(float deltaTime) {
    if(firstFrame) {
        originalPos = holder_->getPosition();
        firstFrame = false;
    }
    animTime += deltaTime;
    if(animTime >= 2.0) {
        animTime = 0.0;
    }
    float animSin = sin(animTime * M_PI);

    glm::vec3 newPos = glm::vec3(originalPos.x, originalPos.y + animSin * holder_->getScale().y, originalPos.z );
    holder_->setPosition(newPos);

}
