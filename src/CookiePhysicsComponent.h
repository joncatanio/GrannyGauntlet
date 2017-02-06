#ifndef COOKIE_PHYSICS_COMPONENT_H
#define COOKIE_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

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
};

#endif