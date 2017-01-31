#include "CookiePhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

CookiePhysicsComponent::CookiePhysicsComponent() {

}

CookiePhysicsComponent::~CookiePhysicsComponent() {

}

void CookiePhysicsComponent::initObjectPhysics() {
    //TODO(nurgan) currently no initial collision test. Can think of a valid case for that, cookie spawns at camera
    gravity = 10.0;
    yVelocity = 0.0;
}

void CookiePhysicsComponent::updateBoundingBox() {
    BoundingBox& boundBox = holder_->boundBox;
    MatrixTransform transform = holder_->transform;

    boundBox.update(transform.getTransform());
}

void CookiePhysicsComponent::updatePhysics(float deltaTime) {
    GameWorld& world = GameManager::instance().getGameWorld();

    glm::vec3 oldPosition = holder_->getPosition();

    if (holder_->getPosition().y > 0.2f) {
        yVelocity -= gravity * deltaTime;
    } else {
        yVelocity = 0.0f;
        holder_->velocity = 0.0f;
    }

    //TODO(nurgan) make the cookie "spin" when it is in the air

    // If we hit anything, stop "forward" movement
    GameObjectType objTypeHit = world.checkCollision(holder_);

    if (objTypeHit == GameObjectType::STATIC_OBJECT || objTypeHit == GameObjectType::DYNAMIC_OBJECT) {
            holder_->velocity = 0.0f;
    }

    glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity * holder_->direction * deltaTime);
    newPosition += glm::vec3(0.0, yVelocity* deltaTime, 0.0);
    holder_->setPosition(newPosition);
    updateBoundingBox();
}
