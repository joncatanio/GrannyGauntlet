#ifndef COOKIE_HIT_BILLBOARD_PHYSICS_COMPONENT
#define COOKIE_HIT_BILLBOARD_PHYSICS_COMPONENT

#include "PhysicsComponent.h"

class MenuBillboardPhysicsComponent : public PhysicsComponent {
public:

    MenuBillboardPhysicsComponent() {}

	~MenuBillboardPhysicsComponent() {}

	void initObjectPhysics() {}

	void updatePhysics(float deltaTime);

	void startDeliveryAnimation() {};

private:

	const double maxLifeTime = 1000.25;

	double currentLifeTime = 0.0;
};

#endif
