#ifndef MATRIX_TRANSFORM_H
#define MATRIX_TRANSFORM_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MatrixTransform {
public:

	MatrixTransform(); 

	~MatrixTransform();

	glm::mat4& getBoundingBoxTransform();

	glm::mat4& getTransform();

   glm::mat4 getRotate();

	void setTranslation(glm::vec3& offset);

	void setScale(glm::vec3& scaleV);

	void setRotate(float angle, const glm::vec3& axis);

   void setCarRot(float angle, const glm::vec3& axis);

   void addRotation(float angle, const glm::vec3& axis);
private:

	glm::mat4 boundingBoxTransform_;
	glm::mat4 transform_;

	glm::mat4 translate_;
	glm::mat4 scale_;
	glm::mat4 rotate_;

   glm::mat4 carTiltRot_;

	void updateTransform();
};

#endif
