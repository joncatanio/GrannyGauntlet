#ifndef BILLBOARD_PHYSICS_COMPONENT
#define BILLBOARD_PHYSICS_COMPONENT

#include "PhysicsComponent.h"

class BillboardPhysicsComponent : public PhysicsComponent {
public:

	BillboardPhysicsComponent();

	~BillboardPhysicsComponent();

	void initObjectPhysics();

	void updatePhysics(float deltaTime);

	void startDeliveryAnimation() {};

private:

	const double maxLifeTime = 1.25;

	double currentLifeTime = 0.0;
};

#endif
