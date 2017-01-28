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

    yVelocity -= gravity * deltaTime;

    // Update position of GameObject (if a collision occurs, will reset to old position and update position again)
    glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity * holder_->direction * deltaTime);
    newPosition += glm::vec3(0.0, yVelocity* deltaTime, 0.0);
    holder_->setPosition(newPosition);
    updateBoundingBox();

    // If we hit someone or we're at the edge of the acceptable "world", then reverse direction
    GameObjectType objTypeHit = world.checkCollision(holder_);

    if (objTypeHit == GameObjectType::STATIC_OBJECT || objTypeHit == GameObjectType::NONSTATIC_OBJECT) {
            holder_->velocity = 0.0f;
            newPosition = oldPosition + (holder_->velocity * holder_->direction * deltaTime);
            newPosition += glm::vec3(0.0, yVelocity* deltaTime, 0.0);
            holder_->setPosition(newPosition);
            updateBoundingBox();
    }
}
