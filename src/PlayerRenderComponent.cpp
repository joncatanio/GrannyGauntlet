#include "PlayerRenderComponent.h"
#include "GameObject.h"
#include "ShaderManager.h"

PlayerRenderComponent::PlayerRenderComponent(std::shared_ptr<Shape> shape,
   const std::string& shaderName, std::shared_ptr<Material> material)
   : RenderComponent(shape, shaderName, material) {

}

PlayerRenderComponent::~PlayerRenderComponent() {}

void PlayerRenderComponent::draw(std::shared_ptr<MatrixStack> P,
   std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
	ShaderManager& shaderManager = ShaderManager::instance();

   //calculateOrientation();
	shaderManager.renderObject(holder_, shaderName_, shape_, material_, P, V, M);
}

void PlayerRenderComponent::calculateOrientation() {
	float cosOfDir = glm::dot(glm::vec3(1.0f, 0.0f, 1.0f), holder_->direction);

	float rotationAngle = glm::acos(cosOfDir);
	rotationAngle = holder_->direction.z < 0 ? -rotationAngle : rotationAngle;

	holder_->setYAxisRotation(rotationAngle);
}
