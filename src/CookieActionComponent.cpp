#include "CookieActionComponent.h"
#include "GameManager.h"
#include "ShaderManager.h"
#include "ShapeManager.h"
#include "MaterialManager.h"
#include <glm/gtx/rotate_vector.hpp>

/*
 * Poll input from the given input handler, and throws a throwable
 * dependent on the input and according constraints.
 */
CookieActionComponent::CookieActionComponent() {

    aimInputComponent = new AimInputComponent();

    ShapeManager& shapeManager = ShapeManager::instance();
    MaterialManager& materialManager = MaterialManager::instance();

    BunnyRenderComponent* bunnyRenderComp = new BunnyRenderComponent(
      shapeManager.getShape("Sphere"), "Phong", materialManager.getMaterial("Brass"));
    BunnyRenderComponent* bunnyRenderComp1 = new BunnyRenderComponent(
      shapeManager.getShape("Sphere"), "Phong", materialManager.getMaterial("Brass"));
    BunnyRenderComponent* bunnyRenderComp2 = new BunnyRenderComponent(
      shapeManager.getShape("Sphere"), "Phong", materialManager.getMaterial("Brass"));
    BunnyRenderComponent* bunnyRenderComp3 = new BunnyRenderComponent(
      shapeManager.getShape("Sphere"), "Phong", materialManager.getMaterial("Brass"));
    BunnyRenderComponent* bunnyRenderComp4 = new BunnyRenderComponent(
      shapeManager.getShape("Sphere"), "Phong", materialManager.getMaterial("Brass"));



    gameObj = std::make_shared<GameObject>(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.1),
            nullptr,
            nullptr,
            bunnyRenderComp,
            nullptr);
    gameObj->initComponents();

    gameObj1 = std::make_shared<GameObject>(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.1),
            nullptr,
            nullptr,
            bunnyRenderComp1,
            nullptr);
    gameObj1->initComponents();

    gameObj2 = std::make_shared<GameObject>(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.1),
            nullptr,
            nullptr,
            bunnyRenderComp2,
            nullptr);
    gameObj2->initComponents();

    gameObj3 = std::make_shared<GameObject>(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.1),
            nullptr,
            nullptr,
            bunnyRenderComp3,
            nullptr);
    gameObj3->initComponents();

    gameObj4 = std::make_shared<GameObject>(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.1),
            nullptr,
            nullptr,
            bunnyRenderComp4,
            nullptr);
    gameObj4->initComponents();
}

CookieActionComponent::~CookieActionComponent() {}

void CookieActionComponent::initActionComponent() {

    GameManager::instance().getGameWorld().addDynamicGameObject(gameObj);
    GameManager::instance().getGameWorld().addDynamicGameObject(gameObj1);
    GameManager::instance().getGameWorld().addDynamicGameObject(gameObj2);
    GameManager::instance().getGameWorld().addDynamicGameObject(gameObj3);
    GameManager::instance().getGameWorld().addDynamicGameObject(gameObj4);

}

void CookieActionComponent::checkAndPerformAction(double deltaTime, double totalTime) {

    GameManager& gameManager = GameManager::instance();
    ShapeManager& shapeManager = ShapeManager::instance();
    MaterialManager& materialManager = MaterialManager::instance();
    glm::vec3 initialScale(0.5f, 0.1f, 0.5f);

    aimInputComponent->pollInput();
    double timeDown = aimInputComponent->pressTime;
    timeDown = std::min(timeDown, 1.0) / 2.0;
    timeDown = 0.5 + timeDown;

    /* Restrict the aim motion to the half-sphere in front of the player. */
    if(aimInputComponent->toggleXRotation){
        // Up and down
        if ((-M_PI / 4.0 <= xRot && xRot <= M_PI / 3.0) ||
            (xRot > M_PI / 3.0 && aimInputComponent->rotationXDirection < 0) ||
            (xRot < -M_PI / 4.0 && aimInputComponent->rotationXDirection > 0)) {
            xRot += aimInputComponent->rotationXDirection * deltaTime;
        }
    }
    if(aimInputComponent->toggleYRotation){
        // Left and right
        if ((-M_PI / 2.0 <= yRot && yRot <= M_PI / 2.0) ||
            (yRot > M_PI / 2.0 && aimInputComponent->rotationYDirection < 0) ||
            (yRot < -M_PI / 2.0 && aimInputComponent->rotationYDirection > 0)) {
            yRot += aimInputComponent->rotationYDirection * deltaTime;
        }
    }

    glm::vec3 upDownRotAxis = glm::cross(holder_->direction, glm::vec3(0.0, 1.0, 0.0));
    glm::vec3 throwDirection = glm::rotate(holder_->direction, xRot, upDownRotAxis);
    throwDirection = glm::rotateY(throwDirection, yRot);

    if (aimInputComponent->toggleThrow) {
        if (totalTime >= previousCookieTime + 0.5) {

            CookiePhysicsComponent *cookiePhysicsComp = new CookiePhysicsComponent();
            BunnyRenderComponent *renderComp = new BunnyRenderComponent(
               shapeManager.getShape("Sphere"), "Phong", materialManager.getMaterial("Obsidian"));

            std::shared_ptr<GameObject> cookieObj = std::make_shared<GameObject>(
                    GameObjectType::DYNAMIC_OBJECT,
                    holder_->getPosition(),
                    throwDirection,
                    (holder_->velocity + startVelocity) * timeDown,
                    initialScale,
                    nullptr,
                    cookiePhysicsComp,
                    renderComp,
                    nullptr);
            cookieObj->initComponents();

            gameManager.getGameWorld().addDynamicGameObject(cookieObj);

            previousCookieTime = totalTime;
        }
    }

    glm::vec3 aimTarget = holder_->getPosition() + throwDirection * 1.0f;
    gameObj->setPosition(aimTarget);

    glm::vec3 aimTarget1 = holder_->getPosition() + throwDirection * 1.5f;
    gameObj1->setPosition(aimTarget1);

    glm::vec3 aimTarget2 = holder_->getPosition() + throwDirection * 0.5f;
    gameObj2->setPosition(aimTarget2);

    glm::vec3 aimTarget3 = holder_->getPosition() + throwDirection * 2.0f;
    gameObj3->setPosition(aimTarget3);

    glm::vec3 aimTarget4 = holder_->getPosition() + throwDirection * 2.5f;
    gameObj4->setPosition(aimTarget4);
}
