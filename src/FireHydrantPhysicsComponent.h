#ifndef FIRE_HYDRANT_PHYSICS_COMPONENT_H
#define FIRE_HYDRANT_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

class FireHydrantPhysicsComponent : public PhysicsComponent {
public:
   FireHydrantPhysicsComponent();

   ~FireHydrantPhysicsComponent();

   void initObjectPhysics();

   void updatePhysics(float deltaTime);

   void updateAnimation(float deltaTime);

private:
   float gravity;
   float yVelocity;

   /* Animation datat */
   bool animated;
   glm::vec3 animRotAxis;
   float animDuration;
   const float animSpeed = 5.0;
};

#endif
