#include "CookieActionComponent.h"
#include "GameManager.h"
#include "ShaderManager.h"
#include "ShapeManager.h"
#include "MaterialManager.h"
#include "AimRenderComponent.h"
#include <glm/gtx/rotate_vector.hpp>

/*
 * Poll input from the given input handler, and throws a throwable
 * dependent on the input and according constraints.
 */
CookieActionComponent::CookieActionComponent() {

    aimInputComponent = new AimInputComponent();

	ShaderManager& shaderManager = ShaderManager::instance();
    ShapeManager& shapeManager = ShapeManager::instance();
    MaterialManager& materialManager = MaterialManager::instance();

    AimRenderComponent* aimRenderComponent = new AimRenderComponent(
      shapeManager.getShape("Arrow"), shaderManager.DefaultShader, materialManager.getMaterial("Brass"));
    AimRenderComponent* aimRenderComponent1 = new AimRenderComponent(
      shapeManager.getShape("Arrow"), shaderManager.DefaultShader, materialManager.getMaterial("Brass"));
    AimRenderComponent* aimRenderComponent2 = new AimRenderComponent(
      shapeManager.getShape("Arrow"), shaderManager.DefaultShader, materialManager.getMaterial("Brass"));
    AimRenderComponent* aimRenderComponent3 = new AimRenderComponent(
      shapeManager.getShape("Arrow"), shaderManager.DefaultShader, materialManager.getMaterial("Brass"));
    AimRenderComponent* aimRenderComponent4 = new AimRenderComponent(
      shapeManager.getShape("Arrow"), shaderManager.DefaultShader, materialManager.getMaterial("Brass"));



    gameObj = std::make_shared<GameObject>(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.15),
            nullptr,
            nullptr,
            aimRenderComponent,
            nullptr);
    gameObj->initComponents();

    gameObj1 = std::make_shared<GameObject>(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.15),
            nullptr,
            nullptr,
            aimRenderComponent1,
            nullptr);
    gameObj1->initComponents();

    gameObj2 = std::make_shared<GameObject>(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.15),
            nullptr,
            nullptr,
            aimRenderComponent2,
            nullptr);
    gameObj2->initComponents();

    gameObj3 = std::make_shared<GameObject>(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.15),
            nullptr,
            nullptr,
            aimRenderComponent3,
            nullptr);
    gameObj3->initComponents();

    gameObj4 = std::make_shared<GameObject>(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.15),
            nullptr,
            nullptr,
            aimRenderComponent4,
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
    double chargeTime = aimInputComponent->getChargeTime();
    timeDown = std::min(timeDown, 1.5) / 2.0;
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
			ShaderManager& shaderManager = ShaderManager::instance();
            CookiePhysicsComponent *cookiePhysicsComp = new CookiePhysicsComponent();
            BunnyRenderComponent *renderComp = new BunnyRenderComponent(
               shapeManager.getShape("Sphere"), shaderManager.DefaultShader, materialManager.getMaterial("Bronze"));

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
            aimInputComponent->pressTime = 0.0;

            // Reset the material of the aiming objects
            gameObj->changeMaterial(materialManager.getMaterial("Brass"));
            gameObj1->changeMaterial(materialManager.getMaterial("Brass"));
            gameObj2->changeMaterial(materialManager.getMaterial("Brass"));
            gameObj3->changeMaterial(materialManager.getMaterial("Brass"));
            gameObj4->changeMaterial(materialManager.getMaterial("Brass"));
            gameObj->setScale(smallScale);
            gameObj1->setScale(smallScale);
            gameObj2->setScale(smallScale);
            gameObj3->setScale(smallScale);
            gameObj4->setScale(smallScale);
        }
    }


    glm::vec3 aimTarget = holder_->getPosition() + throwDirection * 0.5f;
    gameObj->setPosition(aimTarget);
    setAimRotation(gameObj, gameManager );
    if (chargeTime > 0.01 && chargeTime <= 0.3) {
        gameObj->changeMaterial(materialManager.getMaterial("Emerald"));
        gameObj->setScale(largeScale);
    }

    glm::vec3 aimTarget1 = holder_->getPosition() + throwDirection * 1.0f;
    gameObj1->setPosition(aimTarget1);
    setAimRotation(gameObj1, gameManager);
    if (chargeTime > 0.3 && chargeTime <= 0.6) {
        gameObj1->changeMaterial(materialManager.getMaterial("Emerald"));
        gameObj1->setScale(largeScale);
    }

    glm::vec3 aimTarget2 = holder_->getPosition() + throwDirection * 1.5f;
    gameObj2->setPosition(aimTarget2);
    setAimRotation(gameObj2, gameManager);
    if (chargeTime > 0.6 && chargeTime <= 0.9) {
        gameObj2->changeMaterial(materialManager.getMaterial("Emerald"));
        gameObj2->setScale(largeScale);
    }

    glm::vec3 aimTarget3 = holder_->getPosition() + throwDirection * 2.0f;
    gameObj3->setPosition(aimTarget3);
    setAimRotation(gameObj3, gameManager);
    if (chargeTime > 0.9 && chargeTime <= 1.2) {
        gameObj3->changeMaterial(materialManager.getMaterial("Emerald"));
        gameObj3->setScale(largeScale);

    }

    glm::vec3 aimTarget4 = holder_->getPosition() + throwDirection * 2.5f;
    gameObj4->setPosition(aimTarget4);
    setAimRotation(gameObj4, gameManager);
    if (chargeTime > 1.2) {
        gameObj4->changeMaterial(materialManager.getMaterial("Emerald"));
        gameObj4->setScale(largeScale);
    }

}

void CookieActionComponent::setAimRotation(std::shared_ptr <GameObject> obj, GameManager& gameManager) {
    std::shared_ptr<GameObject> player = gameManager.getPlayer();
    float playerRot = player->getYAxisRotation() - player->getOrientAngle() + yRot;

    obj->setYAxisRotation(playerRot);
    obj->addRotation(xRot, glm::vec3(0.0, 0.0, -1.0));
}
