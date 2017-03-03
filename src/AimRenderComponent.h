#ifndef AIM_RENDER_COMPONENT_H
#define AIM_RENDER_COMPONENT_H

#include "RenderComponent.h"

class AimRenderComponent : public RenderComponent {
public:
    AimRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material);

    ~AimRenderComponent();

    void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

    void renderShadow(std::shared_ptr<MatrixStack> M);

private:

};

#endif
