#ifndef SKYBOXRENDERCOMPONENT_H
#define SKYBOXRENDERCOMPONENT_H

#include "RenderComponent.h"
#include "Cubemap.h"

class SkyboxRenderComponent : public RenderComponent {
public:
    SkyboxRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material, std::string path, std::string fileExtension);

    ~SkyboxRenderComponent();

    void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

    void renderShadow(std::shared_ptr<MatrixStack> M);

private:
    Cubemap* cubemap;

};

#endif
