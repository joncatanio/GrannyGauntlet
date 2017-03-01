#ifndef MATRIX_TRANSFORM_H
#define MATRIX_TRANSFORM_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MatrixTransform {
public:

	MatrixTransform(); 

	~MatrixTransform();

	glm::mat4& getTransform();

   glm::mat4 getRotate();

	void setTranslation(glm::vec3& offset);

	void setScale(glm::vec3& scaleV);

   /* Empties the rotation list and pushes an initial rotation. */
	void setRotate(float angle, const glm::vec3& axis);

   /* Adds a rotation matrix to the rotate_ list, this list is used as a stack
      but needs to be iterated through in updateTransform() */
   void addRotation(float angle, const glm::vec3& axis);
private:

	glm::mat4 transform_;

	glm::mat4 translate_;
	glm::mat4 scale_;
	glm::mat4 rotate_;

	void updateTransform();
};

#endif
