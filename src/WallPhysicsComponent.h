#ifndef WALL_PHYSICS_COMPONENT_H
#define WALL_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

class WallPhysicsComponent : public PhysicsComponent {
public:

    WallPhysicsComponent();

    ~WallPhysicsComponent();

    void initObjectPhysics();

    void updatePhysics(float deltaTime);

    void startDeliveryAnimation();

    void updateAnimation(float deltaTime);

private:

    /* Animation data */
    bool animated = false;
    float animTime = 0.0;
    const float animDuration = 2.0;
    glm::vec3 originalPosition;

    float velocity = 0.0;
    float startVelocity = 20.0;
    float gravity = -50.0;

    int numberOfJumps;
    int maxJumps = 2;
};

#endif
