#include "BillboardPhysicsComponent.h"
#include "BillboardRenderComponent.h"
#include "GameObject.h"
#include "GameManager.h"
#include "AimRenderComponent.h"
#include "MarkerPhysicsComponent.h"
#include "ShaderManager.h"
#include "ShapeManager.h"
#include "MaterialManager.h"

GameObject::GameObject(GameObjectType objType,
	glm::vec3 startPosition,
	glm::vec3 startDirection,
	float startVelocity,
	glm::vec3 initialScale,
	InputComponent* input,
	PhysicsComponent* physics,
	RenderComponent* render,
	ActionComponent* action,
	bool deliverable)
	: direction(glm::normalize(startDirection)),
	velocity(startVelocity),
	type(objType),
	toggleMovement(false),
      cookieDeliverable(deliverable),
	orientAngle_(0),
	yRotationAngle_(0),
	render_(render),
	input_(input),
	physics_(physics),
	action_(action) {

	// Set initial position and scale values
	setPosition(startPosition);
	setScale(initialScale);
}

GameObject::~GameObject() {

}

void GameObject::initComponents() {
	if (input_ != NULL) {
	  input_->setGameObjectHolder(shared_from_this());
	}

	glm::vec3 minBoundBoxPt(0.0f, 0.0f, 0.0f);
	glm::vec3 maxBoundBoxPt(0.0f, 0.0f, 0.0f);

	if (render_ != NULL) {
		render_->setGameObjectHolder(shared_from_this());
		minBoundBoxPt = render_->getShape()->getMin();
		maxBoundBoxPt = render_->getShape()->getMax();
	}

	if (physics_ != NULL) {
		physics_->setGameObjectHolder(shared_from_this());
		physics_->initBoundingBox(minBoundBoxPt, maxBoundBoxPt);
		physics_->initObjectPhysics();
	}

	if (action_ != NULL) {
	    action_->setGameObjectHolder(shared_from_this());
	    action_->initActionComponent();
	}

    if(cookieDeliverable) {
        createMarkerObject();
    }
}

void GameObject::createMarkerObject() {
	ShaderManager& shaderManager = ShaderManager::instance();
    ShapeManager& shapeManager = ShapeManager::instance();
    MaterialManager& materialManager = MaterialManager::instance();
    AimRenderComponent* arrowRenderComponent = new AimRenderComponent(
            shapeManager.getShape("Arrow"), shaderManager.DefaultShader, materialManager.getMaterial("Bright Green"));
    MarkerPhysicsComponent* markerPhysicsComponent = new MarkerPhysicsComponent();


    glm::vec3 arrowPos = position_ + glm::vec3(0.0f, scale_.y * 2.0f, 0.0f);

    arrow_ = std::make_shared<GameObject>(GameObjectType::DYNAMIC_OBJECT,
                                          arrowPos,
                                          glm::vec3(0.0f, 1.0f, 0.0f),
                                          0.0f,
                                          scale_ / 2.0f,
                                          nullptr,
                                          markerPhysicsComponent,
                                          arrowRenderComponent,
                                          nullptr
    );

    arrow_->initComponents();
}

glm::vec3& GameObject::getPosition() {
	return position_;
}

glm::vec3& GameObject::getScale() {
	return scale_;
}

void GameObject::setOrientAngle(float orientAngle) {
	static glm::vec3 yAxis(0.0f, 1.0f, 0.0f);

   BoundingBox* objectBB = getBoundingBox();

   if (objectBB != NULL) {
      MatrixTransform orientTransform;
      orientTransform.setRotate(orientAngle, yAxis);
      render_->getShape()->findAndSetMinAndMax(orientTransform.getTransform());
      physics_->initBoundingBox(render_->getShape()->getMin(), render_->getShape()->getMax());
   }

   orientAngle_ = orientAngle;
}

float GameObject::getOrientAngle() {
   return orientAngle_;
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

	BoundingBox* objectBB = getBoundingBox();

   if (objectBB != NULL) {
      MatrixTransform orientTransform;
      orientTransform.setRotate(angle, yAxis);
      render_->getShape()->findAndSetMinAndMax(orientTransform.getTransform());
      physics_->initBoundingBox(render_->getShape()->getMin(), render_->getShape()->getMax());
   }

	yRotationAngle_ = angle;
	transform.setRotate(angle, yAxis);
}

void GameObject::addRotation(float angle, const glm::vec3& axis) {
   transform.addRotation(angle, axis);
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

    if (cookieDeliverable) {
        arrow_->update(deltaTime);
    }
}

void GameObject::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
	if (render_ != NULL) {
		render_->draw(P, M, V);
        if(cookieDeliverable) {
            std::shared_ptr<MatrixStack> M2 = std::make_shared<MatrixStack>();
            arrow_->draw(P, M2, V);
        }
	}
}

void GameObject::renderToShadowMap(std::shared_ptr <MatrixStack> M) {
	if (render_ != NULL) {
		render_->renderShadow(M);
	}
}

void GameObject::performAction(double deltaTime, double totalTime) {
    if (action_ != NULL) {
        action_->checkAndPerformAction(deltaTime, totalTime);
    }
}

void GameObject::spawnHitBillboardEffect(glm::vec3& positionOfHit) {
	static bool areTexturesLoaded = false;
	static std::shared_ptr<Texture> billboardTexture;

	if (!areTexturesLoaded) {
		billboardTexture = std::make_shared<Texture>();
		billboardTexture->loadTexture("../resources/billboard/pow-comic-book-style.jpg", "billboardTex");
	}

	GameManager& gameManager = GameManager::instance();
	GameWorld& world = gameManager.getGameWorld();

	MaterialManager& materialManager = MaterialManager::instance();
	ShapeManager& shapeManager = ShapeManager::instance();
	BillboardRenderComponent* billboardRenderComponent = new BillboardRenderComponent(
		shapeManager.getShape("Cube"), "Billboard", materialManager.getMaterial("Bright Green"), billboardTexture);
	BillboardPhysicsComponent* billboardPhysicsComponent = new BillboardPhysicsComponent();

	std::shared_ptr<GameObject> billboardEffect = std::make_shared<GameObject>(GameObjectType::DYNAMIC_OBJECT,
		positionOfHit,
		glm::vec3(0.0f, 1.0f, 0.0f),
		5.0f,
		glm::vec3(1.0f),
		nullptr,
		billboardPhysicsComponent,
		billboardRenderComponent,
		nullptr
		);
	billboardEffect->initComponents();

	world.addDynamicGameObject(billboardEffect);
}

void GameObject::changeShader(const std::string& newShaderName) {
	if (render_ != NULL) {
		render_->changeShader(newShaderName);
	}
}

RenderComponent* GameObject::getRenderComponent() {
    return render_;
}

bool GameObject::checkIntersection(std::shared_ptr<GameObject> otherObj) {	
	PhysicsComponent* otherObjPhysics = otherObj->physics_;
	if (physics_ != NULL && otherObjPhysics != NULL) {
		return physics_->getBoundingBox().checkIntersection(otherObjPhysics->getBoundingBox());
	}

	return false;
}

BoundingBox* GameObject::getBoundingBox() {
   if (physics_) {
      return &physics_->getBoundingBox();
   }
   return NULL;
}

void GameObject::triggerDeliveryAnimation() {
	physics_->startDeliveryAnimation();
}

GameObjectType GameObject::stringToType(std::string type) {
	if(type == "PLAYER") {
		return GameObjectType::PLAYER;
	} else if(type == "STATIC_OBJECT") {
		return GameObjectType::STATIC_OBJECT;
	} else if(type == "DYNAMIC_OBJECT") {
		return GameObjectType::DYNAMIC_OBJECT;
	} else if(type == "FINISH_OBJECT") {
		return GameObjectType::FINISH_OBJECT;
	} else {
		//default to static object
		return GameObjectType::STATIC_OBJECT;
	}
}
