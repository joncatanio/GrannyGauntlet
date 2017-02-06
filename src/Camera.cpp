#include "Camera.h"
#include "GameObject.h"
#include <glm/gtx/rotate_vector.hpp>

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

/* Eye is not added to LookAt because gaze will ultimately be LA - Eye + Eye
 * therefore we separate the LA completely. `getTarget()` will then add Eye
 * in order to create our view matrix. */
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
   alpha = glm::clamp(alpha, -80.0f, 80.0f);
	alphaRad = alpha * M_PI / 180.0;
}

// Specifically for trackpad input.
void Camera::changeBeta(float deltaBeta) {
	beta += deltaBeta;
	betaRad = beta * M_PI / 180.0;
   /* Keep the player and camera in sync in regards to their orientation.
    * `betaRad` already has a 180 degree rotation baked into it so I just
    * deal with it by subracting `M_PI` from `betaRad`, this lets us let use
    * the player's orientation angle. We still want to rotate the direction
    * vector to point in the direction of the camera. */
   float newOrient = player->getOrientAngle() - (betaRad - M_PI);
   player->setYAxisRotation(newOrient);
   player->direction = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), -betaRad);
}

/* Calculates the angle of the given direction and the original direction vector
 * to determine the rotation angle to apply to the current direction vector.
 * Note: The dot product doesn't specify sign therefore if the z component
 * of the given vector is negative the angle will be negated.
 * Note: If the player is in reverse the base direction for the angle
 * calculation is reversed.
 */
void Camera::calcCamAndPlayerOrient(glm::vec3 dirOrigOrient, bool reverse) {
   glm::vec3 origDirection, unitDir = glm::normalize(dirOrigOrient);
   if (reverse) {
      origDirection = glm::vec3(1.0f, 0.0f, 0.0f);
   } else {
      origDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
   }

   float dotAB = glm::dot(unitDir, origDirection);
   float angle = glm::acos(dotAB);

   angle = unitDir.z < 0 ? -angle : angle;
   beta -= angle;
   betaRad = beta * M_PI / 180.0f;

   float newOrient = player->getOrientAngle() - (betaRad - M_PI);
   player->setYAxisRotation(newOrient);
   player->direction = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), -betaRad);
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
