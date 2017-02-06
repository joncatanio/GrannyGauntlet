#ifndef WALL_PHYSICS_COMPONENT_H
#define WALL_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

class WallPhysicsComponent : public PhysicsComponent {
public:

    WallPhysicsComponent();

    ~WallPhysicsComponent();

    void initObjectPhysics();

    void updatePhysics(float deltaTime);

private:

};

#endif
