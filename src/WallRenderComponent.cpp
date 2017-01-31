#include "WallRenderComponent.h"
#include "GameObject.h"
#include "ShaderManager.h"

WallRenderComponent::WallRenderComponent(std::shared_ptr<Shape> shape, std::shared_ptr<Program> shaderProgram, std::shared_ptr<Material> material)
	: RenderComponent(shape, shaderProgram, material) {

}

WallRenderComponent::~WallRenderComponent() {

}

void WallRenderComponent::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
	ShaderManager& shaderManager = ShaderManager::instance();
	shaderManager.renderObject(holder_, shaderProgram_->name, shape_, material_, P, V, M);
}
