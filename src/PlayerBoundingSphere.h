#ifndef BOUNDING_VOLUME_H
#define BOUNDING_VOLUME_H

#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <algorithm>
#include <float.h>
#include <iostream>

#include "BoundingBox.h"

class PlayerBoundingSphere : public BoundingBox {
public:

	glm::vec3 objCenter_;

	glm::vec3 center_;

	float radius_;

	PlayerBoundingSphere();

	PlayerBoundingSphere(glm::vec3& min, glm::vec3& max);

	~PlayerBoundingSphere();

	// Checks if the BoundingSphere intersects with the passed BoundingBox's coordinates
	bool checkIntersection(std::shared_ptr<BoundingBox> other);

	// Updates the bounding box min, max, and boxPoints based on the passed transform
	// Also, updates the sphere center
	void update(glm::mat4& transform);

	bool isPlayer();
};

#endif
