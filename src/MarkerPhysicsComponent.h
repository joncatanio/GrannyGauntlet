#ifndef MARKER_PHYSICS_COMPONENT_H
#define MARKER_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

class MarkerPhysicsComponent : public PhysicsComponent {
public:

    MarkerPhysicsComponent();

    ~MarkerPhysicsComponent();

    void initObjectPhysics();

    void updatePhysics(float deltaTime);

    void startDeliveryAnimation() {};

    void updateAnimation(float deltaTime);

private:

    /* Animation data */
    bool firstFrame = true;
    float animTime = 0.0;
    const float animDuration = 2.0;
    glm::vec3 originalPos;

};

#endif
