#ifndef BUNNY_PHYSICS_COMPONENT_H
#define BUNNY_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

class BunnyPhysicsComponent : public PhysicsComponent {
public:

	BunnyPhysicsComponent();

	~BunnyPhysicsComponent();

	void initObjectPhysics();

	void updatePhysics(double deltaTime);

	void updateBoundingBox();

private:

};

#endif