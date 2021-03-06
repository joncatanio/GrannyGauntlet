#include "PlayerBoundingSphere.h"

PlayerBoundingSphere::PlayerBoundingSphere() : BoundingBox() {
	objCenter_ = (objMin_ + objMax_) / 2.0f;
	center_ = objCenter_;
	radius_ = abs(glm::length(objMax_ - center_));
}

PlayerBoundingSphere::PlayerBoundingSphere(glm::vec3& min, glm::vec3& max) : BoundingBox(min, max) {
	std::cout << "HERFSXC" << std::endl;
	objCenter_ = (min + max) / 2.0f;
	center_ = objCenter_;
	radius_ = abs(glm::length(max - center_));
}

PlayerBoundingSphere::~PlayerBoundingSphere() {}

// Checks if the BoundingSphere intersects with the passed BoundingBox's coordinates
bool PlayerBoundingSphere::checkIntersection(std::shared_ptr<BoundingBox> other) {

	// Find the closest point
	float x = std::max(other->min_.x, std::min(center_.x, other->max_.x));
	float y = std::max(other->min_.y, std::min(center_.y, other->max_.y));
	float z = std::max(other->min_.z, std::min(center_.z, other->max_.z));

	glm::vec3 closestPointBoxToSphere(x, y, z);

	// Check if the the distance to the closest point is smaller than the radius
	float distance = glm::length(closestPointBoxToSphere - center_);

	return distance < radius_;
}

// Updates the bounding box min, max, and boxPoints based on the passed transform
void PlayerBoundingSphere::update(glm::mat4& transform) {
	BoundingBox::update(transform);
	center_ = transform * glm::vec4(objCenter_, 1.0);
}

bool PlayerBoundingSphere::isPlayer() {
	return true;
}
