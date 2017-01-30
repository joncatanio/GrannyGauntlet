#include "Camera.h"
#include "GameObject.h"

// Ninety degrees in radians
constexpr float ninetyRad = 90.0f * M_PI / 180.0f;

Camera::Camera(GameObject* player)
   : springConstant(25.0f),
   cameraDistance(5.0f),
   cameraVelocity(glm::vec3(0.0f, 0.0f, 0.0f)) {
      // TODO (noj) handle null input at some point.
      this->player = player;
      
      // Calculate damping constant in critically-damped system
      dampingConstant = 2.0f * sqrt(springConstant);
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

   /* Make spring calculations */
   // Ideal resting camera position
   glm::vec3 restDist = player->getPosition() + w * cameraDistance;
   // Displacement from equilibrium
   glm::vec3 displacement = Eye - restDist;
   // Hooke's Law with damping
   glm::vec3 springAccel = (-springConstant * displacement) -
      (dampingConstant * cameraVelocity);

   cameraVelocity += springAccel * deltaTime;
   setEye(Eye + (cameraVelocity * deltaTime));
   
   if (Eye.y < 1) {
      setEye(glm::vec3(Eye.x, 1, Eye.z));
   }
}
