#ifndef CAMERA_H
#define CAMERA_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "BoundingBox.h"
#include "ShaderHelper.h"

class Camera {
public:

	// Scale for the W and A key movements
	const float WSScale = 10.0f;

	// Scale for the A and S key movements
	const float ADScale = 10.0f;

	// TODO(rgarmsen): Add an easy way to toggle collision detection on the camera if desired (mid-game too)
	BoundingBox boundBox;

	Camera();

	~Camera();

	glm::vec3& getEye();

	// Returns a reference to the current look-at vector for the camera
	glm::vec3& getLookAt();

	glm::vec3& getUp();

	glm::vec3 getTarget();

	void setEye(glm::vec3 newEye);

	void setLookAt(glm::vec3 newLA);

	void setUp(glm::vec3 newUp);

	void changeAlpha(float deltaAlpha);

	void changeBeta(float deltaBeta);

	void update(float deltaTime);

private:

	// Vector's the define the camera
	glm::vec3 Eye = glm::vec3(0, 1, 0);
	glm::vec3 LA;
	glm::vec3 Up = glm::vec3(0, 1, 0);

	// Angle values used for pitch and yaw respectively
	float alpha = 0;
	float beta = 180;

	// Radian equivalents for alpha and beta
	float alphaRad = alpha * M_PI / 180.0;
	float betaRad = beta * M_PI / 180.0;
};

#endif
