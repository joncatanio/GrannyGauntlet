#ifndef BILLBOARD_RENDER_COMPONENT_H
#define BILLBOARD_RENDER_COMPONENT_H

#include "ShaderManager.h"

#include "RenderComponent.h"

class BillboardRenderComponent : public RenderComponent {
public:

	BillboardRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material);

	~BillboardRenderComponent();

	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

	void renderShadow(std::shared_ptr<MatrixStack> M);

private:

};

#endif
