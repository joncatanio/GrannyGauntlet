#include "MatrixTransform.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

MatrixTransform::MatrixTransform() :

   transform_(glm::mat4(1.0)),
   translate_(glm::mat4(1.0)),
   scale_(glm::mat4(1.0)),
   rotate_(glm::mat4(1.0)),
   carTiltRot_(glm::mat4(1.0)) {
}

MatrixTransform::~MatrixTransform() {

}

glm::mat4& MatrixTransform::getBoundingBoxTransform() { 
   return boundingBoxTransform_; 
}

glm::mat4& MatrixTransform::getTransform() {
   return transform_;
}

glm::mat4 MatrixTransform::getRotate() {
   return carTiltRot_ * rotate_;
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

void MatrixTransform::setCarRot(float angle, const glm::vec3& axis) {
   carTiltRot_ = glm::rotate(glm::mat4(1.0), angle, axis);

   updateTransform();
}

void MatrixTransform::addRotation(float angle, const glm::vec3& axis) {
   rotate_ *= glm::rotate(glm::mat4(1.0), angle, axis);

   updateTransform();
}

void MatrixTransform::updateTransform() {
   boundingBoxTransform_ = translate_ * scale_;
   transform_ = translate_ * carTiltRot_ * rotate_ * scale_;
}
