#include "WallPhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

WallPhysicsComponent::WallPhysicsComponent() {

}

WallPhysicsComponent::~WallPhysicsComponent() {

}

void WallPhysicsComponent::initObjectPhysics() {
    hasCookie = false;
}

void WallPhysicsComponent::updateBoundingBox() {
    BoundingBox& boundBox = holder_->boundBox;
    MatrixTransform transform = holder_->transform;

    boundBox.update(transform.getTransform());
}

void WallPhysicsComponent::updatePhysics(float deltaTime) {
    // If we hit someone or we're at the edge of the acceptable "world", then reverse direction
    GameWorld& world = GameManager::instance().getGameWorld();
    GameObjectType objTypeHit = world.checkCollision(holder_)->type;
    if (objTypeHit == GameObjectType::DYNAMIC_OBJECT) {
        if (!hasCookie) {
            hasCookie = true;
            //holder_->changeShader("Green");
        }
    }
}
