#ifndef COOKIE_ACTION_COMPONENT_H
#define COOKIE_ACTION_COMPONENT_H

#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include "ActionComponent.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "ShaderManager.h"
#include "AimInputComponent.h"

class Shape;

class CookieActionComponent : public ActionComponent {
public:

    // Constructs a new CookieActionComponent
    CookieActionComponent();

    ~CookieActionComponent();

    void initActionComponent();

    // checks for user input and throws a Cookie
    void checkAndPerformAction(double deltaTime, double totalTime);

private:
    std::shared_ptr<GameObject> gameObj;
    std::shared_ptr<GameObject> gameObj1;
    std::shared_ptr<GameObject> gameObj2;
    std::shared_ptr<GameObject> gameObj3;
    std::shared_ptr<GameObject> gameObj4;
    AimInputComponent* aimInputComponent;
    std::shared_ptr<Shape> cookieShape;
    float xRot = 0.0;
    float yRot = 0.0;
    float startVelocity = 20.0f;
    double previousCookieTime = 0.0;
    glm::vec3 smallScale = glm::vec3(0.15);
    glm::vec3 largeScale = glm::vec3(0.2);

    void setAimRotation(std::shared_ptr<GameObject> obj, GameManager& gameManager);

};

#endif
