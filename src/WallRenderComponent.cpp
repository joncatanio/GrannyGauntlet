#include "WallRenderComponent.h"
#include "GameObject.h"

WallRenderComponent::WallRenderComponent(std::shared_ptr<Shape> shape, std::shared_ptr<Program> shaderProgram, std::shared_ptr<Material> material)
	: RenderComponent(shape, shaderProgram, material) {

}

WallRenderComponent::~WallRenderComponent() {

}

void WallRenderComponent::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
	shaderProgram_->bind();

	// Bind perspective and view tranforms
	glUniformMatrix4fv(progPhong->getUniform("P"), 1, GL_FALSE, P->topMatrix().data());
	glUniformMatrix4fv(progPhong->getUniform("V"), 1, GL_FALSE, V->topMatrix().data());

	// Bind light properties
	glUniform3f(progPhong->getUniform("lightPos"), curLight.x, curLight.y, curLight.z);
	glUniform3f(progPhong->getUniform("lightClr"), curLight.r, curLight.g, curLight.b);

	// Bind material properties
	glUniform3f(progPhong->getUniform("MatAmb"), material_->rAmb, material_->gAmb, material_->bAmb);
	glUniform3f(progPhong->getUniform("MatDif"), material_->rDif, material_->gDif, material_->bDif);
	glUniform3f(progPhong->getUniform("MatSpc"), material_->rSpc, material_->gSpc, material_->bSpc);
	glUniform1f(progPhong->getUniform("MatShiny"), material_->shininess);

	// Set up and bind model transform
	M->pushMatrix();
	M->loadIdentity();

	M->multMatrix(holder_->transform.getTransform());
	glUniformMatrix4fv(progPhong->getUniform("M"), 1, GL_FALSE, M->topMatrix().data());

	// Draw wall
	shape_->draw(shaderProgram_);

	M->popMatrix();

	shaderProgram_->unbind();
}
