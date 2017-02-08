#ifndef PLAYER_PHYSICS_COMPONENT_H
#define PLAYER_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

class PlayerPhysicsComponent : public PhysicsComponent {
public:
   PlayerPhysicsComponent();

   ~PlayerPhysicsComponent();

   void initObjectPhysics();

   void updatePhysics(float deltaTime);

private:
};

#endif
