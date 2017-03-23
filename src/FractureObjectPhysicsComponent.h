#ifndef FRACTURE_OBJECT_PHYSICS_COMPONENT_H
#define FRACTURE_OBJECT_PHYSICS_COMPONENT_H

#include "PhysicsComponent.h"

class FractureObjectPhysicsComponent : public PhysicsComponent {
public:
	FractureObjectPhysicsComponent();

	~FractureObjectPhysicsComponent();

	void initObjectPhysics();

	void updatePhysics(float deltaTime);

	void updateAnimation(float deltaTime);

	void startDeliveryAnimation() {};

private:
	float gravity;
	float yVelocity;

	/* Animation data */
	bool animated;
	glm::vec3 animRotAxis;
	const float animSpeed = 5.0;
};

#endif
