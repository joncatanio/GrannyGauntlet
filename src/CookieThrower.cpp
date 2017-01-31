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

    if (aimInputComponent->toggleThrow) {
        if (totalTime >= previousCookieTime + 0.5) {

            CookiePhysicsComponent *cookiePhysicsComp = new CookiePhysicsComponent();
            BunnyRenderComponent *renderComp = new BunnyRenderComponent(cookieShape,
               "Phong", obsidian);

            glm::vec3 upDownRotAxis = glm::cross(player.direction, glm::vec3(0.0, 1.0, 0.0));
            glm::vec3 throwDirection = glm::rotate(player.direction, xRot, upDownRotAxis);
            throwDirection = glm::rotateY(throwDirection, yRot);

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
}
