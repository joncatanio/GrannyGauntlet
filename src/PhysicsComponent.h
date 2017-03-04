#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <time.h>

#include "BoundingBox.h"
#include "Component.h"
#include "MatrixTransform.h"

class PhysicsComponent : public Component {
public:
	PhysicsComponent() {}

	virtual ~PhysicsComponent() {}

	// Initializes the internal bounding box based off the passed min and max points
	void initBoundingBox(glm::vec3& minBoundPt, glm::vec3& maxBoundPt);

	// Updates the object's bounding box according to the holding object's current transform
	void updateBoundingBox();

	// Returns a reference to the object's bounding box
	BoundingBox& getBoundingBox();

	// Initializes the object's physics specific to the implementing component
	virtual void initObjectPhysics() = 0;

	// Updates the object's physics by the given timestep |deltaTime|
	virtual void updatePhysics(float deltaTime) = 0;

	// starts the delivery animation
	virtual void startDeliveryAnimation() = 0;

private:

	// The bounding box associated with the object
	BoundingBox boundBox_;

};

#endif
