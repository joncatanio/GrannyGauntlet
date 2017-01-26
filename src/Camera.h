#ifndef CAMERA_H
#define CAMERA_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

#include "ShaderHelper.h"

#include "BoundingBox.h"

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

	Eigen::Vector3f& getEye();

	// Returns a reference to the current look-at vector for the camera
	Eigen::Vector3f& getLookAt();

	Eigen::Vector3f& getUp();

	Eigen::Vector3f getTarget();

	void setEye(Eigen::Vector3f newEye);

	void setLookAt(Eigen::Vector3f newLA);

	void setUp(Eigen::Vector3f newUp);

	void changeAlpha(float deltaAlpha);

	void changeBeta(float deltaBeta);

	void update(double deltaTime);

private:

	// Vector's the define the camera
	Eigen::Vector3f Eye = Eigen::Vector3f(0, 1, 0);
	Eigen::Vector3f LA;
	Eigen::Vector3f Up = Eigen::Vector3f(0, 1, 0);

	// Angle values used for pitch and yaw respectively
	float alpha = 0;
	float beta = 180;

	// Radian equivalents for alpha and beta
	float alphaRad = alpha * M_PI / 180.0;
	float betaRad = beta * M_PI / 180.0;
};

#endif
