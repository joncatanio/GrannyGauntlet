#include "BillboardRenderComponent.h"

BillboardRenderComponent::BillboardRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material)
	: RenderComponent(shape, shaderName, material) {
}

BillboardRenderComponent::~BillboardRenderComponent() {

}

void BillboardRenderComponent::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
	ShaderManager& shaderManager = ShaderManager::instance();
	shaderManager.renderBillboard(holder_, shaderName_, shape_, material_, P, V, M);
}

void BillboardRenderComponent::renderShadow(std::shared_ptr <MatrixStack> M) {
	// No shadows on billboard effects currently
}
