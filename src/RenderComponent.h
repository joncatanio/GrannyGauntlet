#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shape.h"
#include "MatrixStack.h"
#include "ShaderHelper.h"

#include "Component.h"

class RenderComponent : public Component {
public:

	// Constructs a new RenderComponent using the passed shader program
	RenderComponent(std::shared_ptr<Shape> shape, std::shared_ptr<Program> shaderProgram, std::shared_ptr<Material> material)
		: shape_(shape), 
		shaderProgram_(shaderProgram),
		material_(material) {}

	virtual ~RenderComponent() {}

	// Returns a shared_ptr to the current shape
	inline std::shared_ptr<Shape> getShape() { return shape_; }

	inline std::shared_ptr<Material> getMaterial() { return material_; }

	inline void setMaterial(std::shared_ptr<Material> newMaterial) { material_ = newMaterial; }

	// Draws the object using the passed Perspective (P), Model (M), and View (V) matrices
	virtual void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) = 0;

protected:

	// Shape information that is needed to draw the object
	std::shared_ptr<Shape> shape_;

	// Shader program data to use with the object
	std::shared_ptr<Program> shaderProgram_;

	// Material that is currently in use for this object
	std::shared_ptr<Material> material_;

private:

};

#endif
