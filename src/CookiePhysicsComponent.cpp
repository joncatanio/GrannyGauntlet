#include "CookiePhysicsComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

CookiePhysicsComponent::CookiePhysicsComponent() {

}

CookiePhysicsComponent::~CookiePhysicsComponent() {

}

void CookiePhysicsComponent::initObjectPhysics() {
    //TODO(nurgan) currently no initial collision test. Can think of a valid case for that, cookie spawns at player
    gravity = 10.0;
    yVelocity = 0.0;
    epsilon = 0.5;
    cookieState = {0, glfwGetTime(), holder_->getPosition()};
}

void CookiePhysicsComponent::updatePhysics(float deltaTime) {
    GameWorld& world = GameManager::instance().getGameWorld();

    glm::vec3 oldPosition = holder_->getPosition();

    glm::vec3 newPosition = holder_->getPosition() + (holder_->velocity * holder_->direction * deltaTime);
    newPosition += glm::vec3(0.0, yVelocity* deltaTime, 0.0);
    holder_->setPosition(newPosition);
    updateBoundingBox();

    if (holder_->getPosition().y > 0.2f) {
        yVelocity -= gravity * deltaTime;
    } else {
        yVelocity = 0.0f;
        holder_->velocity = 0.0f;
    }

    //TODO(nurgan) make the cookie "spin" when it is in the air

    // If we hit anything, stop "forward"
    std::shared_ptr<GameObject> objHit = world.checkCollision(holder_);
    GameObjectType objTypeHit = objHit->type;

    if (objTypeHit == GameObjectType::STATIC_OBJECT || objTypeHit == GameObjectType::DYNAMIC_OBJECT) {

        BoundingBox* objBB = objHit->getBoundingBox();
        BoundingBox* cookieBB = holder_->getBoundingBox();

        glm::vec3 n;

        //check on which side of the bounding box of the object the cookie hit and create normal for reflection
        if((objBB->min_.x - epsilon <= cookieBB->max_.x && objBB->min_.x + epsilon >= cookieBB->max_.x) ||
                (objBB->max_.x - epsilon <= cookieBB->min_.x && objBB->max_.x + epsilon >= cookieBB->min_.x)) {
            n = glm::vec3(1.0, 0.0, 0.0);
        }
        if((objBB->min_.y - epsilon <= cookieBB->max_.y && objBB->min_.y + epsilon >= cookieBB->max_.y) ||
                (objBB->max_.y - epsilon <= cookieBB->min_.y && objBB->max_.y + epsilon >= cookieBB->min_.y)){
            n = glm::vec3(0.0, 1.0, 0.0);
        }
        if((objBB->min_.z - epsilon <= cookieBB->max_.z && objBB->min_.z + epsilon >= cookieBB->max_.z) ||
                (objBB->max_.z - epsilon <= cookieBB->min_.z && objBB->max_.z + epsilon >= cookieBB->min_.z)){
            n = glm::vec3(0.0, 0.0, 1.0);
        }

        holder_->direction = glm::reflect(holder_->direction, n);

        newPosition = oldPosition + (holder_->velocity * holder_->direction * deltaTime);
        newPosition += glm::vec3(0.0, yVelocity* deltaTime, 0.0);
        holder_->setPosition(newPosition);
        updateBoundingBox();

        // The color change has to be done here,
        // due to the bouncing off, the cookie never really hits the object.
        // TODO(nurgan) implement checking for object type and only change color if it is a "house"
        if( objHit->getRenderComponent()->getShader().compare("Green")) {
            objHit->changeShader("Green");
            cookieState.hits++;
            cookieState.hitPosition = holder_->getPosition();
            float score = calculateScore();
            GameManager& gameManager = GameManager::instance();
            gameManager.reportScore(score);
            gameManager.increaseTime(score/100.0);
        }

    }


}

float CookiePhysicsComponent::calculateScore() {

    //first house hit 500, second 1000, ...
    float score = 500.0 * cookieState.hits;

    //the distance multiplier
    float distance = glm::distance(
            glm::vec3(cookieState.launchPosition.x, 0.0, cookieState.launchPosition.z),
            glm::vec3(cookieState.hitPosition.x, 0.0, cookieState.hitPosition.z) );

    //TODO(nurgan) score and multiplier tweaking. distance 30 equals ~ the width of the road now.
    float distanceMultiplier = 1.0;
    if(distance > 30.0) {
        distanceMultiplier = 2.0;
    } else if(distance > 25.0) {
        distanceMultiplier = 1.75;
    } else if(distance > 20.0) {
        distanceMultiplier = 1.5;
    } else if(distance > 15.0) {
        distanceMultiplier = 1.25;
    }

    score *= distanceMultiplier;

    return score;

}