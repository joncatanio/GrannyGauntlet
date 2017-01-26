#include "GameObject.h"
#include "GameManager.h"

GameObject::GameObject(GameObjectType objType,
	Eigen::Vector3f startPosition,
	Eigen::Vector3f startDirection,
	float startVelocity,
	Eigen::Vector3f initialScale,
	InputHandler* input,
	PhysicsComponent* physics,
	RenderComponent* render)
	: type(objType),
	direction(startDirection.normalized()),
	velocity(startVelocity),
	input_(input),
	physics_(physics),
	render_(render) {

	// Set initial position and scale values
	setPosition(startPosition);
	setScale(initialScale);

	calculateAndSetInitialRotation();

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

Eigen::Vector3f& GameObject::getPosition() {
	return position_;
}

Eigen::Vector3f& GameObject::getScale() {
	return scale_;
}

float GameObject::getYAxisRotation() {
	return yRotationAngle_;
}

void GameObject::setPosition(Eigen::Vector3f& newPosition) {
	position_ = newPosition;
	transform.setTranslation(position_);
}

void GameObject::setScale(Eigen::Vector3f& newScale) {
	scale_ = newScale;
	transform.setScale(scale_);
}

void GameObject::setYAxisRotation(float angle) {
	static Eigen::Vector3f yAxis(0.0f, 1.0f, 0.0f);

	yRotationAngle_ = angle;
	transform.setRotate(angle, yAxis);
}

void GameObject::calculateAndSetInitialRotation() {
	if (velocity > 0.0f) {
		float cosOfDir = Eigen::Vector3f(1.0f, 0.0f, 0.0f).dot(direction);

		// If the dot between axis and orthog. vec. is negative then we know the angle is negative (since right-hand rule)
		Eigen::Vector3f crossVec = Eigen::Vector3f(1.0f, 0.0f, 0.0f).cross(direction);
		float angleDirection = Eigen::Vector3f(0.0f, 1.0f, 0.0f).dot(crossVec);
		if (angleDirection >= 0) {
			setYAxisRotation((acos(cosOfDir) * 180.0f / M_PI) + 180.0f);
		}
		else {
			setYAxisRotation((-acos(cosOfDir) * 180.0f / M_PI) + 180.0f);
		}
	}
	else {
		setYAxisRotation(0.0f);
	}
}

void GameObject::changeMaterial(std::shared_ptr<Material> newMaterial) {
	if (newMaterial != render_->getMaterial()) {
		render_->setMaterial(newMaterial);
	}
}

void GameObject::update(double deltaTime) {
	if (input_ != NULL) {
		input_->handleInput();
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
