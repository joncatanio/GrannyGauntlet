#ifndef COOKIE_RENDER_COMPONENT_H
#define COOKIE_RENDER_COMPONENT_H

#include "RenderComponent.h"

class CookieRenderComponent : public RenderComponent {
public:
	CookieRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material);

	~CookieRenderComponent();

	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

	void renderShadow(std::shared_ptr<MatrixStack> M);

private:

};

#endif
