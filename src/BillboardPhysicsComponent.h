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

	const double maxLifeTime = 3.0;

	double currentLifeTime = 0.0;
<<<<<<< 678a7567b1118afe499b003031fbbe261ca9cc44
=======

>>>>>>> Initial Billboard implementation with 3d objects
};

#endif
