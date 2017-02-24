#ifndef PLAYER_RENDER_COMPONENT_H
#define PLAYER_RENDER_COMPONENT_H

#include "RenderComponent.h"

class PlayerRenderComponent : public RenderComponent {
public:
   PlayerRenderComponent(std::shared_ptr<Shape> shape,
      const std::string& shaderName,
      std::shared_ptr<Material> material);

   ~PlayerRenderComponent();

	void draw(std::shared_ptr<MatrixStack> P,
      std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

	void renderShadow(std::shared_ptr<MatrixStack> M);

private:

};

#endif
