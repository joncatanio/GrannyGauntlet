#include "PhysicsComponent.h"

#include "GameObject.h"

void PhysicsComponent::initBoundingBox(glm::vec3& minBoundPt, glm::vec3& maxBoundPt) {
	boundBox_ = std::make_shared<BoundingBox>(BoundingBox(minBoundPt, maxBoundPt));
	updateBoundingBox();
}

void PhysicsComponent::updateBoundingBox() {
	MatrixTransform& transform = holder_->transform;
	boundBox_->update(transform.getBoundingBoxTransform());
}

std::shared_ptr<BoundingBox> PhysicsComponent::getBoundingBox() {
	return boundBox_;
}
