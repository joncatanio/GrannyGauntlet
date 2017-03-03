#include "BunnyRenderComponent.h"
#include "GameObject.h"
#include "ShaderManager.h"

BunnyRenderComponent::BunnyRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material)
	: RenderComponent(shape, shaderName, material) {
}

BunnyRenderComponent::~BunnyRenderComponent() {

}

void BunnyRenderComponent::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
	ShaderManager& shaderManager = ShaderManager::instance();
	shaderManager.renderObject(holder_, shaderName_, shape_, material_, P, V, M);
}

void BunnyRenderComponent::renderShadow(std::shared_ptr <MatrixStack> M) {
	ShaderManager& shaderManager = ShaderManager::instance();
	shaderManager.renderShadowPass(holder_, shape_, M);
}
