#include "PhysicsComponent.h"

#include "GameObject.h"

void PhysicsComponent::initBoundingBox(glm::vec3& minBoundPt, glm::vec3& maxBoundPt) {
	boundBox_ = BoundingBox(minBoundPt, maxBoundPt);
	updateBoundingBox();
}

void PhysicsComponent::updateBoundingBox() {
	MatrixTransform transform = holder_->transform;
	boundBox_.update(transform.getTransform());
}

BoundingBox& PhysicsComponent::getBoundingBox() {
	return boundBox_;
}
