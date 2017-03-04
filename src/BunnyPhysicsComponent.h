#ifndef BUNNY_PHYSICS_COMPONENT_H
#define BUNNY_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

class BunnyPhysicsComponent : public PhysicsComponent {
public:

	BunnyPhysicsComponent();

	~BunnyPhysicsComponent();

	void initObjectPhysics();

	void updatePhysics(float deltaTime);

	void startDeliveryAnimation() {};

private:

	// Sets the initial rotation of the bunny according to it's direction vector
	void setupInitialRotation();

};

#endif