#ifndef MATRIX_TRANSFORM_H
#define MATRIX_TRANSFORM_H

#define _USE_MATH_DEFINES
#include <math.h>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

// TODO(rgarmsen2295): Separate into .cpp file or make inline
class MatrixTransform {
public:

	MatrixTransform() 
		: transform_(Eigen::Matrix4f::Identity()),
		translate_(Eigen::Matrix4f::Identity()),
		scale_(Eigen::Matrix4f::Identity()),
		rotate_(Eigen::Matrix4f::Identity()) {}

	~MatrixTransform() {}

	Eigen::Matrix4f& getTransform() { return transform_; }

	void setTranslation(Eigen::Vector3f& translate) {
		translate_ = Eigen::Matrix4f::Identity();
		translate_(0, 3) = translate(0);
		translate_(1, 3) = translate(1);
		translate_(2, 3) = translate(2);

		updateTransform();
	}

	void setScale(Eigen::Vector3f& scale) {
		scale_ = Eigen::Matrix4f::Identity();
		scale_(0, 0) = scale(0);
		scale_(1, 1) = scale(1);
		scale_(2, 2) = scale(2);

		updateTransform();
	}

	// TODO(rgarmsen2295): Add seperate functions and member variables to allow for x, y, AND z rotation at the same time
	void setRotate(float rotateAngle, const Eigen::Vector3f& rotateAxis)
	{
		rotate_ = Eigen::Matrix4f::Identity();
		rotate_.block<3, 3>(0, 0) = Eigen::AngleAxisf(rotateAngle * M_PI / 180.0f, rotateAxis).toRotationMatrix();

		updateTransform();
	}

	void setRotate(Eigen::AngleAxisf angleAxis) {
		rotate_ = Eigen::Matrix4f::Identity();
		rotate_.block<3, 3>(0, 0) = angleAxis.toRotationMatrix();

		updateTransform();
	}

private:

	Eigen::Matrix4f transform_;

	Eigen::Matrix4f translate_;
	Eigen::Matrix4f scale_;
	Eigen::Matrix4f rotate_;

	// TODO(rgarmsen): Add options on how to do the transform update (e.g. different orders)
	void updateTransform() {
		transform_ = translate_ * rotate_ * scale_;
	}
};

#endif
