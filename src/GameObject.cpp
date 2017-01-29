#include "GameObject.h"
#include "GameManager.h"

GameObject::GameObject(GameObjectType objType,
	glm::vec3 startPosition,
	glm::vec3 startDirection,
	float startVelocity,
	glm::vec3 initialScale,
	InputComponent* input,
	PhysicsComponent* physics,
	RenderComponent* render)
	: type(objType),
	direction(glm::normalize(startDirection)),
	velocity(startVelocity),
   toggleXMovement(false),
   toggleZMovement(false),
	input_(input),
	physics_(physics),
	render_(render) {

	// Set initial position and scale values
	setPosition(startPosition);
	setScale(initialScale);

	//calculateAndSetInitialRotation();

   if (input_ != NULL) {
      input_->setGameObjectHolder(this);
   }

	if (render_ != NULL) {
		render_->setGameObjectHolder(this);
		boundBox = BoundingBox(render_->getShape()->getMin(), render_->getShape()->getMax());
	}
	else {

		// If no render component, set BoundingBox to min(0,0,0) -> max(0,0,0)
		boundBox = BoundingBox();
	}

	if (physics_ != NULL) {
		physics_->setGameObjectHolder(this);
		physics_->updateBoundingBox();
		physics_->initObjectPhysics();
	}
	else {
		// TODO(rgarmsen2295): Move all bounding box stuff to base physics component class
		boundBox.update(transform.getTransform());
	}
}

GameObject::~GameObject() {

}

glm::vec3& GameObject::getPosition() {
	return position_;
}

glm::vec3& GameObject::getScale() {
	return scale_;
}

float GameObject::getYAxisRotation() {
	return yRotationAngle_;
}

void GameObject::setPosition(glm::vec3& newPosition) {
	position_ = newPosition;
	transform.setTranslation(position_);
}

void GameObject::setScale(glm::vec3& newScale) {
	scale_ = newScale;
	transform.setScale(scale_);
}

void GameObject::setYAxisRotation(float angle) {
	static glm::vec3 yAxis(0.0f, 1.0f, 0.0f);

	yRotationAngle_ = angle;
	transform.setRotate(angle, yAxis);
}

void GameObject::calculateAndSetInitialRotation() {
	if (velocity > 0.0f) {
		float cosOfDir = glm::dot(glm::vec3(-1.0f, 0.0f, 0.0f), direction);
		float rotationAngle = (glm::acos(cosOfDir) * 180.0f / M_PI);

		rotationAngle = direction.z < 0 ? -rotationAngle : rotationAngle;
		setYAxisRotation(rotationAngle);
	}

	// TODO(rgarmsen2295): Old rotate implementation - delete when above working
	/*if (velocity > 0.0f) {
		float cosOfDir = glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), direction);

		// If the dot between axis and orthog. vec. is negative then we know the angle is negative (since right-hand rule)
		glm::vec3 crossVec = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), direction);
		float angleDirection = glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), crossVec);
		if (angleDirection >= 0) {
			setYAxisRotation((glm::acos(cosOfDir) * 180.0f / M_PI) + 180.0f);
		}
		else {
			setYAxisRotation((glm::acos(cosOfDir) * 180.0f / M_PI) + 180.0f);
		}
	}
	else {
		setYAxisRotation(0.0f);
	}*/
}

void GameObject::changeMaterial(std::shared_ptr<Material> newMaterial) {
	if (newMaterial != render_->getMaterial()) {
		render_->setMaterial(newMaterial);
	}
}

void GameObject::update(double deltaTime) {
	if (input_ != NULL) {
		input_->pollInput();
	}

	if (physics_ != NULL) {
		physics_->updatePhysics(deltaTime);
	}
}

void GameObject::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
	if (render_ != NULL) {
		render_->draw(P, M, V);
	}
}
