#ifndef BUNNY_RENDER_COMPONENT_H
#define BUNNY_RENDER_COMPONENT_H

#include "RenderComponent.h"

class BunnyRenderComponent : public RenderComponent {
public:
	BunnyRenderComponent(std::shared_ptr<Shape> shape, std::shared_ptr<Program> shaderProgram, std::shared_ptr<Material> material);

	~BunnyRenderComponent();

	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

private:

};

#endif
