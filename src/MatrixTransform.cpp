#include "MatrixTransform.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

MatrixTransform::MatrixTransform() :
   transform_(glm::mat4(1.0)),
   translate_(glm::mat4(1.0)),
   scale_(glm::mat4(1.0)) {

   rotate_.push_front(glm::mat4(1.0));
}

MatrixTransform::~MatrixTransform() {

}

glm::mat4& MatrixTransform::getTransform() {
   return transform_;
}

glm::mat4 MatrixTransform::getRotate() {
   glm::mat4 rotation(glm::mat4(1.0));

   for (glm::mat4 rot : rotate_) {
      rotation *= rot;
   }

   return rotation;
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
   rotate_.clear();
   rotate_.push_front(glm::rotate(glm::mat4(1.0), angle, axis));

   updateTransform();
}

void MatrixTransform::pushRotation(float angle, const glm::vec3& axis) {
   rotate_.push_front(glm::rotate(glm::mat4(1.0), angle, axis));

   updateTransform();
}

void MatrixTransform::popRotation() {
   rotate_.pop_front();

   updateTransform();
}

void MatrixTransform::updateTransform() {
   transform_ = translate_;

   for (glm::mat4 rotation : rotate_) {
      transform_ *= rotation;
   }

   transform_ *= scale_;
}
