#ifndef MATRIX_TRANSFORM_H
#define MATRIX_TRANSFORM_H

#define _USE_MATH_DEFINES
#include <math.h>

// TODO(rgarmsen2295): Separate into .cpp file or make inline
class MatrixTransform {
public:

	MatrixTransform() 
		: transform_(glm::mat4(1.0)),
		translate_(glm::mat4(1.0)),
		scale_(glm::mat4(1.0)),
		rotate_(glm::mat4(1.0)) {}

	~MatrixTransform() {}

	glm::mat4& getTransform() { return transform_; }

	void setTranslation(glm::vec3& offset) {
		translate_ = glm::translate(glm::mat4(1.0), offset);

		updateTransform();
	}

	void setScale(glm::vec3& scaleV) {
		scale_ = glm::scale(glm::mat4(1.0), scaleV);

		updateTransform();
	}

	// TODO(rgarmsen2295): Add seperate functions and member variables to allow for x, y, AND z rotation at the same time
	void setRotate(float angle, const glm::vec3& axis)
	{
		rotate_ = glm::rotate(glm::mat4(1.0), angle, axis);

		updateTransform();
	}

private:

	glm::mat4 transform_;

	glm::mat4 translate_;
	glm::mat4 scale_;
	glm::mat4 rotate_;

	// TODO(rgarmsen): Add options on how to do the transform update (e.g. different orders)
	void updateTransform() {
		transform_ = translate_ * rotate_ * scale_;
	}
};

#endif
