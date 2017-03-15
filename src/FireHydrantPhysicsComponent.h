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

    void startDeliveryAnimation() {};

private:
   float gravity;
   float yVelocity;
   bool untouched = true;

   /* Animation data */
   bool animated;
   glm::vec3 animRotAxis;
   const float animSpeed = 5.0;
};

#endif
