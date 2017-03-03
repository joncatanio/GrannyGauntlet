#include "BoundingBox.h"

BoundingBox::BoundingBox()
	: objMin_(glm::vec3(0.0f, 0.0f, 0.0f)),
	objMax_(glm::vec3(0.0f, 0.0f, 0.0f)),
	min_(glm::vec3(0.0f, 0.0f, 0.0f)),
	max_(glm::vec3(0.0f, 0.0f, 0.0f)) {
	for (int i = 0; i < 8; ++i) {
		objBoxPoints[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		boxPoints[i] = objBoxPoints[i];
	}
}

BoundingBox::BoundingBox(glm::vec3& min, glm::vec3& max)
	: objMin_(min),
	objMax_(max),
	min_(min),
	max_(max) {
	objBoxPoints[0] = boxPoints[0] = min_;
	objBoxPoints[1] = boxPoints[1] = max_;
	objBoxPoints[2] = boxPoints[2] = glm::vec3(max_.x, max_.y, min_.z);
	objBoxPoints[3] = boxPoints[3] = glm::vec3(min_.x, max_.y, min_.z);
	objBoxPoints[4] = boxPoints[4] = glm::vec3(min_.x, min_.y, max_.z);
	objBoxPoints[5] = boxPoints[5] = glm::vec3(max_.x, min_.y, max_.z);
	objBoxPoints[6] = boxPoints[6] = glm::vec3(max_.x, min_.y, min_.z);
	objBoxPoints[7] = boxPoints[7] = glm::vec3(min_.x, max_.y, max_.z);
}

/* 
 * Basic idea + circular versus box collisions sourced from here
 * https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
 */
bool BoundingBox::checkIntersection(BoundingBox& other) {
	return (this->min_.x <= other.max_.x && this->max_.x >= other.min_.x) &&
		(this->min_.y <= other.max_.y && this->max_.y >= other.min_.y) &&
		(this->min_.z <= other.max_.z && this->max_.z >= other.min_.z);
}

glm::vec3 BoundingBox::calcReflNormal(BoundingBox& other) {
   glm::vec3 normal = glm::vec3(0.0, 0.0, 0.0);
   float epsilon = 1.5;

   //check on which side of the bounding box of the object the cookie hit and create normal for reflection
   if((this->min_.x - epsilon <= other.max_.x && this->min_.x + epsilon >= other.max_.x) ||
           (this->max_.x - epsilon <= other.min_.x && this->max_.x + epsilon >= other.min_.x)) {
       normal.x = 1.0;
   }
   if((this->min_.y - epsilon <= other.max_.y && this->min_.y + epsilon >= other.max_.y) ||
           (this->max_.y - epsilon <= other.min_.y && this->max_.y + epsilon >= other.min_.y)){
       normal.y = 1.0;
   }
   if((this->min_.z - epsilon <= other.max_.z && this->min_.z + epsilon >= other.max_.z) ||
           (this->max_.z - epsilon <= other.min_.z && this->max_.z + epsilon >= other.min_.z)){
       normal.z = 1.0;
   }

   return glm::normalize(normal);
}

void BoundingBox::update(glm::mat4& transform) {
	float minX, minY, minZ;
	float maxX, maxY, maxZ;

	minX = minY = minZ = FLT_MAX;
	maxX = maxY = maxZ = -FLT_MAX;

	// TODO(rgarmsen2295): Optimize this
	for (int i = 0; i < 8; ++i) {
		glm::vec3 tempPoint((transform * glm::vec4(objBoxPoints[i], 1.0f)));
		boxPoints[i] = tempPoint;

		if (boxPoints[i].x < minX) minX = boxPoints[i].x;
		if (boxPoints[i].x > maxX) maxX = boxPoints[i].x;

		if (boxPoints[i].y < minY) minY = boxPoints[i].y;
		if (boxPoints[i].y > maxY) maxY = boxPoints[i].y;

		if (boxPoints[i].z < minZ) minZ = boxPoints[i].z;
		if (boxPoints[i].z > maxZ) maxZ = boxPoints[i].z;
	}

	min_ = glm::vec3(minX, minY, minZ);
	max_ = glm::vec3(maxX, maxY, maxZ);
}
