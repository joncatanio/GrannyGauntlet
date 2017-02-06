#include "WallPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

WallPhysicsComponent::WallPhysicsComponent() {

}

WallPhysicsComponent::~WallPhysicsComponent() {

}

void WallPhysicsComponent::initObjectPhysics() {

}

void WallPhysicsComponent::updateBoundingBox() {
    BoundingBox& boundBox = holder_->boundBox;
    MatrixTransform transform = holder_->transform;

    boundBox.update(transform.getTransform());
}

void WallPhysicsComponent::updatePhysics(float deltaTime) {

}
