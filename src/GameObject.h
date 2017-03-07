#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

#include "BoundingBox.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "ActionComponent.h"
#include "MaterialManager.h"

enum class GameObjectType { PLAYER, STATIC_OBJECT, DYNAMIC_OBJECT, FINISH_OBJECT };

class GameObject : public std::enable_shared_from_this<GameObject> {
public:

	// Direct object properties
	glm::vec3 direction;
	float velocity;
	MatrixTransform transform;
	GameObjectType type;

    // Properties for player moveable objects
    bool toggleMovement;

	// Can a Cookie be delivered to this object
	bool cookieDeliverable;

   // Object is being fractured
   bool fracture;

	// Constructs a new GameObject using the given components.
	// A NULL component will not be used
	GameObject(GameObjectType objType,
		glm::vec3 startPosition,
		glm::vec3 startDirection,
		float startVelocity,
		glm::vec3 initialScale,
		InputComponent* input,
		PhysicsComponent* physics,
		RenderComponent* render,
		ActionComponent* action,
		bool deliverable = false);

    ~GameObject();

    void initComponents();

    glm::vec3& getPosition();

    glm::vec3& getScale();

    void setOrientAngle(float orientAngle);

    float getOrientAngle();

    float getYAxisRotation();

    void setPosition(glm::vec3& newPosition);

    void setScale(glm::vec3& newScale);

    void setYAxisRotation(float angle);

    void addRotation(float angle, const glm::vec3& axis);

    void calculateAndSetInitialRotation();

    void changeMaterial(std::shared_ptr<Material> newMaterial);

    RenderComponent* getRenderComponent();

	// Performs any non-render related updates to the object
	void update(double deltaTime);

    // Performs any render/draw updates necessary for the object
    void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

	// Renders the Object to the shadow map
	void renderToShadowMap(std::shared_ptr<MatrixStack> M);

    // Perform the any actions that are bound to the object, if any and if applicable at that moment
    void performAction(double deltaTime, double totalTime);

	// Changes the active shader for the object
	void changeShader(const std::string& newShaderName);

	// Checks if the object intersects with the passed object
	bool checkIntersection(std::shared_ptr<GameObject> otherObj);

	// Starts the delivery animation of the Game Object, if any
	void triggerDeliveryAnimation();

    static GameObjectType stringToType(std::string type);

   // Sets the fragment direction vector usually from Shape.cpp this also
   // initializes the starting position of each fragment.
   void setFragmentDirs(std::shared_ptr<std::vector<glm::vec3>> fragDirs);

   // Gets the fragment direction vector for the fractured object
   std::shared_ptr<std::vector<glm::vec3>> getFragmentDirs();

   // Gets the fragment position vector for the fractured object
   std::shared_ptr<std::vector<glm::vec3>> getFragmentPos();

    // Returns the BoundingBox associated with the object if it exists, otherwise returns |NULL|
    // TRY TO AVOID USING THIS IF POSSIBLE, SHOULD BE REMOVED AT SOME POINT, BB LOGIC ONLY IN PHYSICSCOMPONENT
    BoundingBox* getBoundingBox();

private:

    // The current position of the object in world space
    glm::vec3 position_;

    // The current scale of the object relative to it's original size
    glm::vec3 scale_;

    // The orientation angle to orient the object correctly from it's original
    float orientAngle_;

    // The current y-axis rotation of the object relative to it's original orientation
    float yRotationAngle_;

    // Creates the Game Object for the "Quest Maker" arrow
    void createMarkerObject();

    // Stuff necessary to draw object
    RenderComponent* render_;

    // Stuff necessary to control object
    InputComponent* input_;
    
	// Physics Component that handles collision reactions
	PhysicsComponent *physics_;

    // Action Component to control actions performed by objects
    ActionComponent* action_;

    // Arrow "Quest Marker" for deliverable palces
    std::shared_ptr<GameObject> arrow_;

    // When an object is fractured this maintains the direction of each fragment
    std::shared_ptr<std::vector<glm::vec3>> fragDirs_;

    // The fractured objects current position.
    std::shared_ptr<std::vector<glm::vec3>> fragPos_;

};

#endif
