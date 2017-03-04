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
    glm::vec3 originalScale;

};

#endif
