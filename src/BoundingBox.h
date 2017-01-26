#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

#include <float.h>
#include <iostream>

class BoundingBox {
public:

	// The "lowest" point on the original object data (before transforms)
	Eigen::Vector3f objMin_;

	// The "highest" point on the original object data (before transforms)
	Eigen::Vector3f objMax_;

	// Array of all the points representing the original BoundingBox (before transforms)
	Eigen::Vector3f objBoxPoints[8];

	// The "lowest" point on the BoundingBox
	Eigen::Vector3f min_;

	// The "highest" point on the BoundingBox
	Eigen::Vector3f max_;

	// Array of all the points representing the current BoundingBox
	Eigen::Vector3f boxPoints[8];

	BoundingBox();

	BoundingBox(Eigen::Vector3f& min, Eigen::Vector3f& max);

	~BoundingBox() {}

	// Checks if the BoundingBox intersects with the passed BoundingBox's coordinates
	bool checkIntersection(BoundingBox& other);

	// Updates the bounding box min, max, and boxPoints based on the passed transform
	void update(Eigen::Matrix4f& transform);
};

#endif
