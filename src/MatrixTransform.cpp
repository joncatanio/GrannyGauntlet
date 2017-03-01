#include "MatrixTransform.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

MatrixTransform::MatrixTransform() :
   transform_(glm::mat4(1.0)),
   translate_(glm::mat4(1.0)),
   scale_(glm::mat4(1.0)),
   rotate_(glm::mat4(1.)) {
}

MatrixTransform::~MatrixTransform() {

}

glm::mat4& MatrixTransform::getTransform() {
   return transform_;
}

glm::mat4 MatrixTransform::getRotate() {
   return rotate_;
}

void MatrixTransform::setTranslation(glm::vec3& offset) {
   translate_ = glm::translate(glm::mat4(1.0), offset);

   updateTransform();
}

void MatrixTransform::setScale(glm::vec3& scaleV) {
   scale_ = glm::scale(glm::mat4(1.0), scaleV);

   updateTransform();
}

void MatrixTransform::setRotate(float angle, const glm::vec3& axis) {
   rotate_ = glm::rotate(glm::mat4(1.0), angle, axis);

   updateTransform();
}

void MatrixTransform::addRotation(float angle, const glm::vec3& axis) {
   rotate_ *= glm::rotate(glm::mat4(1.0), angle, axis);

   updateTransform();
}

void MatrixTransform::updateTransform() {
   transform_ = translate_ * rotate_ * scale_;
}
