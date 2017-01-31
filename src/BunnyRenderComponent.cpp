#include "BunnyRenderComponent.h"
#include "GameObject.h"
#include "ShaderManager.h"

BunnyRenderComponent::BunnyRenderComponent(std::shared_ptr<Shape> shape, std::shared_ptr<Program> shaderProgram, std::shared_ptr<Material> material)
	: RenderComponent(shape, shaderProgram, material) {
}

BunnyRenderComponent::~BunnyRenderComponent() {

}

void BunnyRenderComponent::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
	ShaderManager& shaderManager = ShaderManager::instance();
	shaderManager.renderObject(holder_, shaderProgram_->name, shape_, material_, P, V, M);

	/*shaderManager.bindShader(shaderProgram_->name);

	// TODO(rgarmsen2295): Add boilerplate to standard method call - probably in ShaderManager or similar
	// Bind perspective and view tranforms
	glUniformMatrix4fv(shaderProgram_->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(shaderProgram_->getUniform("V"), 1, GL_FALSE, glm::value_ptr(V->topMatrix()));

	// Bind light properties
	glUniform3f(shaderProgram_->getUniform("lightPos"), curLight.x, curLight.y, curLight.z);
	glUniform3f(shaderProgram_->getUniform("lightClr"), curLight.r, curLight.g, curLight.b);

	// Bind material properties
	glUniform3f(shaderProgram_->getUniform("MatAmb"), material_->rAmb, material_->gAmb, material_->bAmb);
	glUniform3f(shaderProgram_->getUniform("MatDif"), material_->rDif, material_->gDif, material_->bDif);
	glUniform3f(shaderProgram_->getUniform("MatSpc"), material_->rSpc, material_->gSpc, material_->bSpc);
	glUniform1f(shaderProgram_->getUniform("MatShiny"), material_->shininess);

	// Set up and bind model transform
	M->pushMatrix();
	M->loadIdentity();

	M->translate(holder_->getPosition());
	M->scale(holder_->getScale());
	M->rotate(holder_->getYAxisRotation(), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(shaderProgram_->getUniform("M"), 1, GL_FALSE, glm::value_ptr(M->topMatrix()));

	// Draw bunny
	shape_->draw(shaderProgram_);

	M->popMatrix();

	shaderManager.unbindShader();*/
}
