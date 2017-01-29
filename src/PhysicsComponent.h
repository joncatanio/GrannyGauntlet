#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "Component.h"
#include <time.h>

class PhysicsComponent : public Component {
public:
	PhysicsComponent() {}

	virtual ~PhysicsComponent() {}

	// Initializes the object's physics specific to the implementing component
	virtual void initObjectPhysics() = 0;

	// Updates the object's physics by the given timestep |deltaTime|
	virtual void updatePhysics(float deltaTime) = 0;

	// Updates the object's bounding box according to it's current MatrixStack
	virtual void updateBoundingBox() = 0;

private:

};

#endif
