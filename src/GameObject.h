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
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"

enum class GameObjectType { PLAYER, STATIC_OBJECT, DYNAMIC_OBJECT, NO_OBJECT };

class GameObject {
public:

	// Direct object properties
	glm::vec3 direction;
	float velocity;
	MatrixTransform transform;
	BoundingBox boundBox;
	GameObjectType type;

   // Properties for player moveable objects
   bool toggleMovement;

	// Constructs a new GameObject using the given components.
	// A NULL component will not be used
	GameObject(GameObjectType objType,
		glm::vec3 startPosition,
		glm::vec3 startDirection,
		float startVelocity,
		glm::vec3 initialScale,
		InputComponent* input,
		PhysicsComponent* physics,
		RenderComponent* render);

	~GameObject();

	glm::vec3& getPosition();

	glm::vec3& getScale();

   void setOrientAngle(float orientAngle);

   float getOrientAngle();

	float getYAxisRotation();

	void setPosition(glm::vec3& newPosition);

	void setScale(glm::vec3& newScale);

	void setYAxisRotation(float angle);

	void calculateAndSetInitialRotation();

	void changeMaterial(std::shared_ptr<Material> newMaterial);

	// Performs any non-render related updates to the object
	void update(double deltaTime);

	// Performs any render/draw updates necessary for the object
	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

private:

	// The current position of the object in world space
	glm::vec3 position_;

	// The current scale of the object relative to it's original size
	glm::vec3 scale_;

   // The orientation angle to orient the object correctly from it's original
   float orientAngle_;

	// The current y-axis rotation of the object relative to it's original orientation
	float yRotationAngle_;

	// Stuff necessary to draw object
	RenderComponent* render_;

	// Stuff necessary to control object
	InputComponent* input_;

	// Physics Component that handles collision reactions
	PhysicsComponent *physics_;
};

#endif
