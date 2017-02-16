#ifndef FIRE_HYDRANT_PHYSICS_COMPONENT_H
#define FIRE_HYDRANT_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

class FireHydrantPhysicsComponent : public PhysicsComponent {
public:
   FireHydrantPhysicsComponent();

   ~FireHydrantPhysicsComponent();

   void initObjectPhysics();

   void updatePhysics(float deltaTime);

private:
   float gravity;
   float yVelocity;
};

#endif
