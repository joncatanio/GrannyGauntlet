#include "CookieThrower.h"
#include "GameManager.h"
#include "ShaderManager.h"
#include <glm/gtx/rotate_vector.hpp>

/*
 * Poll input from the given input handler, and throws a throwable
 * dependent on the input and according constraints.
 */
CookieThrower::CookieThrower() {
    aimInputComponent = new AimInputComponent();
    cookieShape = std::make_shared<Shape>();
    cookieShape->loadMesh(RESOURCE_DIR + "sphere.obj");
    cookieShape->resize();
    cookieShape->init();

    //
    ShaderManager& shaderManager = ShaderManager::instance();
    std::shared_ptr<Program> progPhong = shaderManager.getShaderProgram("Phong");

    static std::shared_ptr<Shape> shape = std::make_shared<Shape>();
    static bool hasLoaded = false;

    if (!hasLoaded) {
        shape->loadMesh(RESOURCE_DIR + "sphere.obj");
        shape->resize();
        shape->init();
        hasLoaded = true;
    }

    BunnyRenderComponent* bunnyRenderComp = new BunnyRenderComponent(shape, progPhong, brass);
    BunnyRenderComponent* bunnyRenderComp1 = new BunnyRenderComponent(shape, progPhong, brass);
    BunnyRenderComponent* bunnyRenderComp2 = new BunnyRenderComponent(shape, progPhong, brass);

    gameObj = new GameObject(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.1),
            NULL,
            NULL,
            bunnyRenderComp);

    gameObj1 = new GameObject(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.1),
            NULL,
            NULL,
            bunnyRenderComp1);

    gameObj2 = new GameObject(
            GameObjectType::DYNAMIC_OBJECT,
            glm::vec3(0.0),
            glm::vec3(0.0),
            0.0,
            glm::vec3(0.1),
            NULL,
            NULL,
            bunnyRenderComp2);

    GameManager::instance().getGameWorld().addDynamicGameObject(gameObj);
    GameManager::instance().getGameWorld().addDynamicGameObject(gameObj1);
    GameManager::instance().getGameWorld().addDynamicGameObject(gameObj2);

}

CookieThrower::~CookieThrower() {}

void CookieThrower::pollAndThrow(double deltaTime, double totalTime) {

    GameManager& gameManager = GameManager::instance();
    Camera& camera = gameManager.getCamera();
    GameObject& player = gameManager.getPlayer();
    ShaderManager& shaderManager = ShaderManager::instance();
    glm::vec3 initialScale(0.5f, 0.1f, 0.5f);

    aimInputComponent->pollInput();

    if(aimInputComponent->toggleXRotation){
        xRot += aimInputComponent->velocity * deltaTime;
    }
    if(aimInputComponent->toggleYRotation){
        yRot += aimInputComponent->velocity * deltaTime;
    }

    glm::vec3 upDownRotAxis = glm::cross(player.direction, glm::vec3(0.0, 1.0, 0.0));
    glm::vec3 throwDirection = glm::rotate(player.direction, xRot, upDownRotAxis);
    throwDirection = glm::rotateY(throwDirection, yRot);

    if (aimInputComponent->toggleThrow) {
        if (totalTime >= previousCookieTime + 0.5) {

            CookiePhysicsComponent *cookiePhysicsComp = new CookiePhysicsComponent();
            BunnyRenderComponent *renderComp = new BunnyRenderComponent(cookieShape,
               "Phong", obsidian);

            GameObject *cookieObj = new GameObject(
                    GameObjectType::DYNAMIC_OBJECT,
                    player.getPosition(),
                    throwDirection,
                    startVelocity,
                    initialScale,
                    NULL,
                    cookiePhysicsComp,
                    renderComp);

            gameManager.getGameWorld().addDynamicGameObject(cookieObj);

            previousCookieTime = totalTime;
        }
    }

    glm::vec3 aimTarget = player.getPosition() + throwDirection * 1.5f;
    gameObj->setPosition(aimTarget);

    glm::vec3 aimTarget1 = player.getPosition() + throwDirection * 1.0f;
    gameObj1->setPosition(aimTarget1);

    glm::vec3 aimTarget2 = player.getPosition() + throwDirection * 0.5f;
    gameObj2->setPosition(aimTarget2);

}
