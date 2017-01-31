#include "WallRenderComponent.h"
#include "GameObject.h"
#include "ShaderManager.h"

WallRenderComponent::WallRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material)
	: RenderComponent(shape, shaderName, material) {

}

WallRenderComponent::~WallRenderComponent() {

}

void WallRenderComponent::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
	ShaderManager& shaderManager = ShaderManager::instance();
	shaderManager.renderObject(holder_, shaderName_, shape_, material_, P, V, M);
}
