#include "BoundingBox.h"

BoundingBox::BoundingBox()
	: objMin_(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
	objMax_(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
	min_(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
	max_(Eigen::Vector3f(0.0f, 0.0f, 0.0f)) {
	for (int i = 0; i < 8; ++i) {
		objBoxPoints[i] = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
		boxPoints[i] = objBoxPoints[i];
	}
}

BoundingBox::BoundingBox(Eigen::Vector3f& min, Eigen::Vector3f& max) 
	: objMin_(min),
	objMax_(max),
	min_(min),
	max_(max) {
	objBoxPoints[0] = boxPoints[0] = Eigen::Vector3f(min_);
	objBoxPoints[1] = boxPoints[1] = Eigen::Vector3f(max_);
	objBoxPoints[2] = boxPoints[2] = Eigen::Vector3f(max_.x(), max.y(), min.z());
	objBoxPoints[3] = boxPoints[3] = Eigen::Vector3f(min_.x(), max.y(), min.z());
	objBoxPoints[4] = boxPoints[4] = Eigen::Vector3f(min_.x(), min.y(), max.z());
	objBoxPoints[5] = boxPoints[5] = Eigen::Vector3f(max_.x(), min.y(), max.z());
	objBoxPoints[6] = boxPoints[6] = Eigen::Vector3f(max_.x(), min.y(), min.z());
	objBoxPoints[7] = boxPoints[7] = Eigen::Vector3f(min_.x(), max.y(), max.z());
}

/* 
 * Basic idea + circular versus box collisions sourced from here
 * https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
 */
bool BoundingBox::checkIntersection(BoundingBox& other) {
	return (this->min_.x() <= other.max_.x() && this->max_.x() >= other.min_.x()) &&
		(this->min_.y() <= other.max_.y() && this->max_.y() >= other.min_.y()) &&
		(this->min_.z() <= other.max_.z() && this->max_.z() >= other.min_.z());
}

void BoundingBox::update(Eigen::Matrix4f& transform) {
	float minX, minY, minZ;
	float maxX, maxY, maxZ;

	minX = minY = minZ = FLT_MAX;
	maxX = maxY = maxZ = -FLT_MAX;

	// TODO(rgarmsen2295): Optimize this
	for (int i = 0; i < 8; ++i) {
		Eigen::Vector4f tempPoint((transform * Eigen::Vector4f(objBoxPoints[i].x(), objBoxPoints[i].y(), objBoxPoints[i].z(), 1.0f)));
		boxPoints[i] = Eigen::Vector3f(tempPoint.x(), tempPoint.y(), tempPoint.z());

		if (boxPoints[i].x() < minX) minX = boxPoints[i].x();
		if (boxPoints[i].x() > maxX) maxX = boxPoints[i].x();

		if (boxPoints[i].y() < minY) minY = boxPoints[i].y();
		if (boxPoints[i].y() > maxY) maxY = boxPoints[i].y();

		if (boxPoints[i].z() < minZ) minZ = boxPoints[i].z();
		if (boxPoints[i].z() > maxZ) maxZ = boxPoints[i].z();
	}

	min_ = Eigen::Vector3f(minX, minY, minZ);
	max_ = Eigen::Vector3f(maxX, maxY, maxZ);
}
