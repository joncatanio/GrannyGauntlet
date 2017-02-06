#ifndef CAMERA_H
#define CAMERA_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "BoundingBox.h"
#include "ShaderHelper.h"

class GameObject;

class Camera {
public:

	// Scale for the W and A key movements
	const float WSScale = 10.0f;

	// Scale for the A and S key movements
	const float ADScale = 10.0f;

	Camera(GameObject* player);

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

   void calcFollowOrient(glm::vec3 dirOrigOrient);

	void update(float deltaTime);

private:
   // The player the camera is following
   GameObject* player;

	// Vector's the define the camera
	glm::vec3 Eye = glm::vec3(0, 2, 0);
	glm::vec3 LA;
	glm::vec3 Up = glm::vec3(0, 1, 0);

   // Weak spring values
   float springConstant;
   float dampingConstant;
   float cameraDistance;
   glm::vec3 cameraVelocity;

	// Angle values used for pitch and yaw respectively
	float alpha = -15;
	float beta = 180;

	// Radian equivalents for alpha and beta
	float alphaRad = alpha * M_PI / 180.0;
	float betaRad = beta * M_PI / 180.0;
};

#endif
