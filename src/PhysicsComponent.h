#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "Component.h"
#include <time.h>

class PhysicsComponent : public Component {
public:
	PhysicsComponent() {}

	virtual ~PhysicsComponent() {}

	virtual void initObjectPhysics() = 0;

	virtual void updatePhysics(double deltaTime) = 0;

	virtual void updateBoundingBox() = 0;

private:

};

#endif
