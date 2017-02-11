#ifndef COOKIE_PHYSICS_COMPONENT_H
#define COOKIE_PHYSICS_COMPONENT_H

#include <vector>
#include "PhysicsComponent.h"

typedef struct {
    int hits;
    double launchTime;
    glm::vec3 launchPosition;
    std::vector<glm::vec3> hitPositions;
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
    float distanceTraveled();
};

#endif