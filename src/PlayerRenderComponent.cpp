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

	if (holder_->canMove) {
		shaderManager.renderObject(holder_, shaderName_, shape_, material_, P, V, M);
	}
}

void PlayerRenderComponent::renderShadow(std::shared_ptr <MatrixStack> M) {
    ShaderManager& shaderManager = ShaderManager::instance();

	if (holder_->canMove) {
		shaderManager.renderShadowPass(holder_, shape_, M);
	}
}