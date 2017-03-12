#ifndef PLAYER_PHYSICS_COMPONENT_H
#define PLAYER_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

class PlayerPhysicsComponent : public PhysicsComponent {
public:
   PlayerPhysicsComponent();

   ~PlayerPhysicsComponent();

   void initBoundingBox(glm::vec3& minBoundPt, glm::vec3& maxBoundPt);

   void initObjectPhysics();

   void updatePhysics(float deltaTime);

    void startDeliveryAnimation() {};

private:
};

#endif
