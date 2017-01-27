#include "Camera.h"

// Ninety degrees in radians
constexpr float ninetyRad = 90.0f * M_PI / 180.0f;

Camera::Camera()
	: boundBox() {
		Eigen::Vector3f minCameraBox(-1.0f, -1.0, -1.0f);
		Eigen::Vector3f maxCameraBox(1.0f, 1.0f, 1.0f);

		boundBox = BoundingBox(minCameraBox, maxCameraBox);
	}

Camera::~Camera() {}

Eigen::Vector3f& Camera::getEye() {
	return Eye;
}

Eigen::Vector3f& Camera::getLookAt() {
	Eigen::Vector3f tempLA(cos(alphaRad) * cos(betaRad), sin(alphaRad), cos(alphaRad) * cos(ninetyRad - betaRad));
	LA = tempLA;

	return LA;
}

Eigen::Vector3f& Camera::getUp() {
	return Up;
}

Eigen::Vector3f Camera::getTarget() {
	return getEye() + getLookAt();
}

void Camera::setEye(Eigen::Vector3f newEye) {
	Eye = newEye;

	// Update bounding box with new camera position
	boundBox.min_ = boundBox.objMin_ + newEye;
	boundBox.max_ = boundBox.objMax_ + newEye;
}

void Camera::setLookAt(Eigen::Vector3f newLA) {
	LA = newLA;
}

void Camera::setUp(Eigen::Vector3f newUp) {
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

void Camera::update(double deltaTime) {
	Eigen::Vector3f gazeVec = LA;
	gazeVec.normalize();

	Eigen::Vector3f w = -gazeVec;

	Eigen::Vector3f upCrossW = Up.cross(w);
	upCrossW.normalize();

	Eigen::Vector3f u = upCrossW;

	int letterState = glfwGetKey(window, GLFW_KEY_W);
	int arrowState = glfwGetKey(window, GLFW_KEY_UP);
	if (letterState == GLFW_PRESS || arrowState == GLFW_PRESS) {

		// Move in the neg dir along the w camera axis (forward) 
		setEye(Eye - (WSScale * w * deltaTime));
	}

	letterState = glfwGetKey(window, GLFW_KEY_A);
	arrowState = glfwGetKey(window, GLFW_KEY_LEFT);
	if (letterState == GLFW_PRESS || arrowState == GLFW_PRESS) {

		// Move in the neg dir along the u camera axis (left)
		setEye(Eye - (ADScale * u * deltaTime));
	}

	letterState = glfwGetKey(window, GLFW_KEY_S);
	arrowState = glfwGetKey(window, GLFW_KEY_DOWN);
	if (letterState == GLFW_PRESS || arrowState == GLFW_PRESS) {

		// Move in the pos dir along the w camera axis (backwards)
		setEye(Eye + (WSScale * w * deltaTime));
	}

	letterState = glfwGetKey(window, GLFW_KEY_D);
	arrowState = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (letterState == GLFW_PRESS || arrowState == GLFW_PRESS) {

		// Move in the pos dir along the u camera axis (right)
		setEye(Eye + (ADScale * u * deltaTime));
	}

	// Don't allow the player to go below the ground
	if (Eye.y() < 1) {
		setEye(Eigen::Vector3f(Eye.x(), 1, Eye.z()));
	}
}
