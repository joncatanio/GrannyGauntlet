#include "AimRenderComponent.h"
#include "GameObject.h"
#include "ShaderManager.h"

AimRenderComponent::AimRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material)
        : RenderComponent(shape, shaderName, material) {
}

AimRenderComponent::~AimRenderComponent() {

}

void AimRenderComponent::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
    ShaderManager& shaderManager = ShaderManager::instance();
    shaderManager.renderObject(holder_, shaderName_, shape_, material_, P, V, M);
}

void AimRenderComponent::renderShadow(std::shared_ptr <MatrixStack> M) {
    // don't do anything, the aim shouldn't create a shadow
}


