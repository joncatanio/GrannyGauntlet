#ifndef WALL_RENDER_COMPONENT_H
#define WALL_RENDER_COMPONENT_H

#include "RenderComponent.h"

class FireHydrantRenderComponent : public RenderComponent {
public:
	FireHydrantRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material);

	~FireHydrantRenderComponent();

	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

	void renderShadow(std::shared_ptr<MatrixStack> M);

private:

};

#endif
