#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "MatrixTransform.h"

#include "BoundingBox.h"
#include "InputHandler.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"

enum class GameObjectType { CAMERA, STATIC_OBJECT, NONSTATIC_OBJECT, NO_OBJECT };

class GameObject {
public:

	// Direct object properties
	Eigen::Vector3f direction;
	float velocity;
	MatrixTransform transform;
	BoundingBox boundBox;
	GameObjectType type;

	// Constructs a new GameObject using the given components.
	// A NULL component will not be used
	GameObject(GameObjectType objType,
		Eigen::Vector3f startPosition,
		Eigen::Vector3f startDirection,
		float startVelocity,
		Eigen::Vector3f initialScale,
		InputHandler* input,
		PhysicsComponent* physics,
		RenderComponent* render);

	~GameObject();

	Eigen::Vector3f& getPosition();

	Eigen::Vector3f& getScale();

	float getYAxisRotation();

	void setPosition(Eigen::Vector3f& newPosition);

	void setScale(Eigen::Vector3f& newScale);

	void setYAxisRotation(float angle);

	void calculateAndSetInitialRotation();

	void changeMaterial(std::shared_ptr<Material> newMaterial);

	// Performs any non-render related updates to the object
	void update(double deltaTime);

	// Performs any render/draw updates necessary for the object
	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

private:

	// The current position of the object in world space
	Eigen::Vector3f position_;

	// The current scale of the object relative to it's original size
	Eigen::Vector3f scale_;

	// The current y-axis rotation of the object relative to it's original orientation
	float yRotationAngle_;

	// Stuff necessary to draw object
	RenderComponent* render_;

	// Stuff necessary to control object
	InputHandler* input_;

	// Physics Component that handles collision reactions
	PhysicsComponent *physics_;
};

#endif
