#ifndef COOKIE_PHYSICS_COMPONENT_H
#define COOKIE_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

typedef struct {
    int hits;
    double launchTime;
    glm::vec3 launchPosition;
    glm::vec3 hitPosition;
} CookieState;

class CookiePhysicsComponent : public PhysicsComponent {
public:

    CookiePhysicsComponent();

    ~CookiePhysicsComponent();

    void initObjectPhysics();

    void updatePhysics(float deltaTime);

private:
    float gravity;
    float yVelocity;
    float epsilon;
    CookieState cookieState;
    float calculateScore();
};

#endif