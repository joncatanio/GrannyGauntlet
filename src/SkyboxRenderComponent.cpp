#include "SkyboxRenderComponent.h"
#include "GameObject.h"
#include "ShaderManager.h"

SkyboxRenderComponent::SkyboxRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material, std::string path, std::string fileExtension)
        : RenderComponent(shape, shaderName, material) {
    cubemap = new Cubemap(path, fileExtension);
    cubemap->loadCubemap();
}

SkyboxRenderComponent::~SkyboxRenderComponent() {

}

void SkyboxRenderComponent::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
    ShaderManager& shaderManager = ShaderManager::instance();
    cubemap->bind();
    shaderManager.renderObject(holder_, shaderName_, shape_, material_, P, V, M);
    cubemap->unbind();
}