#ifndef SKYBOXRENDERCOMPONENT_H
#define SKYBOXRENDERCOMPONENT_H

#include "RenderComponent.h"
#include "Cubemap.h"

class SkyboxRenderComponent : public RenderComponent {
public:
    SkyboxRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material);

    ~SkyboxRenderComponent();

    void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

private:
    Cubemap* cubemap;

};

#endif
