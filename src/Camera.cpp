#include "Camera.h"
#include "GameObject.h"

// Ninety degrees in radians
constexpr float ninetyRad = 90.0f * M_PI / 180.0f;

Camera::Camera(GameObject* player)
	: boundBox() {
      // TODO (noj) handle null input at some point.
      this->player = player;
		glm::vec3 minCameraBox(-1.0f, -1.0f, -1.0f);
		glm::vec3 maxCameraBox(1.0f, 1.0f, 1.0f);

		boundBox = BoundingBox(minCameraBox, maxCameraBox);
	}

Camera::~Camera() {}

glm::vec3& Camera::getEye() {
	return Eye;
}

glm::vec3& Camera::getLookAt() {
   glm::vec3 tempLA(cos(alphaRad) * cos(betaRad), sin(alphaRad),
      cos(alphaRad) * cos(ninetyRad - betaRad));
	LA = tempLA;

	return LA;
}

glm::vec3& Camera::getUp() {
	return Up;
}

glm::vec3 Camera::getTarget() {
	return getEye() + getLookAt();
}

void Camera::setEye(glm::vec3 newEye) {
	Eye = newEye;

	// Update bounding box with new camera position
	boundBox.min_ = boundBox.objMin_ + newEye;
	boundBox.max_ = boundBox.objMax_ + newEye;
}

void Camera::setLookAt(glm::vec3 newLA) {
	LA = newLA;
}

void Camera::setUp(glm::vec3 newUp) {
	Up = newUp;
}

void Camera::changeAlpha(float deltaAlpha) {
	alpha += deltaAlpha;

	// Prevents cross product explosions
	if (alpha >= 80) {
		alpha = 80;
	}
	else if (alpha <= -80) {
		alpha = -80;
	}

	alphaRad = alpha * M_PI / 180.0;
}

void Camera::changeBeta(float deltaBeta) {
	beta += deltaBeta;
	betaRad = beta * M_PI / 180.0;
}

void Camera::update(float deltaTime) {
	glm::vec3 gazeVec = LA;
	glm::normalize(gazeVec);

	glm::vec3 w = -gazeVec;

	glm::vec3 upCrossW = glm::cross(Up, w);
	glm::normalize(upCrossW);

	glm::vec3 u = upCrossW;

   setEye(player->getPosition() + w * 5.0f);
   if (Eye.y < 1) {
      setEye(glm::vec3(Eye.x, 1, Eye.z));
   }
}
