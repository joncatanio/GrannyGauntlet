#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <float.h>
#include <iostream>

class BoundingBox {
public:

	// The "lowest" point on the original object data (before transforms)
	glm::vec3 objMin_;

	// The "highest" point on the original object data (before transforms)
	glm::vec3 objMax_;

	// Array of all the points representing the original BoundingBox (before transforms)
	glm::vec3 objBoxPoints[8];

	// The "lowest" point on the BoundingBox
	glm::vec3 min_;

	// The "highest" point on the BoundingBox
	glm::vec3 max_;

	// Array of all the points representing the current BoundingBox
	glm::vec3 boxPoints[8];

	BoundingBox();

	BoundingBox(glm::vec3& min, glm::vec3& max);

	~BoundingBox() {}

	// Checks if the BoundingBox intersects with the passed BoundingBox's coordinates
	virtual bool checkIntersection(BoundingBox& other);

   // Checks which side of `this` bounding box was hit and creates normal for reflection
   glm::vec3 calcReflNormal(BoundingBox& other);

	// Updates the bounding box min, max, and boxPoints based on the passed transform
	void update(glm::mat4& transform);
};

#endif
