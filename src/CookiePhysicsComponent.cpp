#include "CookiePhysicsComponent.h"

#include "GameManager.h"
#include "MaterialManager.h"
#include "AudioManager.h"
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
    cookieState = {0, glfwGetTime(), holder_->getPosition(), std::vector<glm::vec3>(), 0.0};
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

    if(animated){
        animRotAxis = glm::cross(holder_->direction, glm::vec3(0, -1, 0));
        updateAnimation(deltaTime);
    }


    // If we hit anything, stop "forward"
    std::vector<std::shared_ptr<GameObject>> objsHit = world.checkCollision(holder_);
    if (!objsHit.empty()) {
        std::shared_ptr<GameObject> objHit = objsHit[0];
        GameObjectType objTypeHit = objHit->type;

        if (objTypeHit == GameObjectType::STATIC_OBJECT || objTypeHit == GameObjectType::DYNAMIC_OBJECT) {

            std::shared_ptr<BoundingBox> objBB = objHit->getBoundingBox();
            std::shared_ptr<BoundingBox> cookieBB = holder_->getBoundingBox();
            MaterialManager materialManager = MaterialManager::instance();

            glm::vec3 normal = objBB->calcReflNormal(cookieBB, 1.5f);
            holder_->direction = glm::reflect(holder_->direction, normal);

            newPosition = oldPosition + (holder_->velocity * holder_->direction * deltaTime);
            newPosition += glm::vec3(0.0, yVelocity* deltaTime, 0.0);
            holder_->setPosition(newPosition);
            updateBoundingBox();

            cookieState.hits++;
            cookieState.hitPositions.push_back(holder_->getPosition());
            GameManager& gameManager = GameManager::instance();
            if( objHit->cookieDeliverable) {
   
				// Score effects due to hit
                float score = calculateScore();
                if(gameManager.isInHellMode()) {
                    score = 666.0f;
                }

                gameManager.reportScore(score);

                float timeBump = ((score - cookieState.scored)/500.0) + 1.0;
                cookieState.scored += score;

				// Visual effects due to hit
				objHit->triggerDeliveryAnimation();
				objHit->changeShader("Default");
				objHit->spawnCookieHitBillboardEffect(holder_->getPosition(), score);

				// Play sound effect.
				AudioManager& audioManager = AudioManager::instance();
				audioManager.playEffect("Boing");

				// Other game state changes due to hit
                if(!gameManager.isInHellMode()) {
                    gameManager.increaseTime(timeBump);
                }
                objHit->cookieDeliverable = false;
            }
        } else if(objTypeHit == GameObjectType::PORTAL_TO_HELL) {
            GameManager::instance().enterHell();
        }
    }
}

float CookiePhysicsComponent::calculateScore() {

    //first house hit 500, second 1000, ...
    float score = 500.0 * cookieState.hits;

    //the distance multiplier
    float distance = distanceTraveled();

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

float CookiePhysicsComponent::distanceTraveled() {
    // there is at least one hit
    float distance = glm::distance(
            glm::vec3(cookieState.launchPosition.x, 0.0, cookieState.launchPosition.z),
            glm::vec3(cookieState.hitPositions[0].x, 0.0, cookieState.hitPositions[0].z) );

    // go through all further hits
    for(unsigned int i = 1; i < cookieState.hitPositions.size(); i++) {
        distance += glm::distance(
                glm::vec3(cookieState.hitPositions[i-1].x, 0.0, cookieState.hitPositions[i-1].z),
                glm::vec3(cookieState.hitPositions[i].x, 0.0, cookieState.hitPositions[i].z) );
    }

    return distance;
}

void CookiePhysicsComponent::updateAnimation(float deltaTime) {
    if (holder_->velocity == 0.0f) {
        animated = false;
    } else {
        holder_->addRotation(deltaTime * animSpeed, animRotAxis);
    }
}
