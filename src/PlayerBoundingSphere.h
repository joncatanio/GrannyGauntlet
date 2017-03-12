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

	PlayerBoundingSphere() : BoundingBox() {
		objCenter_ = (objMin_ + objMax_) / 2.0f;
		center_ = objCenter_;
		radius_ = abs(glm::length(objMax_ - center_));
	}

	PlayerBoundingSphere(glm::vec3& min, glm::vec3& max) : BoundingBox(min, max) {
		std::cout << "HERFSXC" << std::endl;
		objCenter_ = (min + max) / 2.0f;
		center_ = objCenter_;
		radius_ = abs(glm::length(max - center_));
	}

	~PlayerBoundingSphere() {}

	// Checks if the BoundingBox intersects with the passed BoundingBox's coordinates
	bool checkIntersection(BoundingBox& other) {
		std::cout << "HERE" << std::endl;
		// get box closest point to sphere center by clamping
		float x = std::max(other.min_.x, std::min(center_.x, other.max_.x));
		float y = std::max(other.min_.y, std::min(center_.y, other.max_.y));
		float z = std::max(other.min_.z, std::min(center_.z, other.max_.z));

		float distance = std::sqrt((x - center_.x) * (x - center_.x) +
			(y - center_.y) * (y - center_.y) +
			(z - center_.z) * (z - center_.z));

		return distance < radius_;
	}

	// Updates the bounding box min, max, and boxPoints based on the passed transform
	void update(glm::mat4& transform) {
		BoundingBox::update(transform);

		center_ = transform * glm::vec4(objCenter_, 1.0);
	}
};

#endif
