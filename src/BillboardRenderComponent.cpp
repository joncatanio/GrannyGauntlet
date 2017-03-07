#include "BillboardRenderComponent.h"

BillboardRenderComponent::BillboardRenderComponent(std::shared_ptr<Shape> shape, const std::string& shaderName, std::shared_ptr<Material> material, std::shared_ptr<Texture> billboardTexture)
	: RenderComponent(shape, shaderName, material),
	billboardTexture_(billboardTexture) {
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// Send the position array to the GPU
	glGenBuffers(1, &posBufIDRef);

	static GLfloat billboardVerts[] = {
		-1, -1, 0.0,
		-1, 1, 0.0,
		1, -1, 0.0,
		1, 1, 0.0
	};
	glBindBuffer(GL_ARRAY_BUFFER, posBufIDRef);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &(billboardVerts[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &texBufIDRef);

	static GLfloat billboardTex[] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};
	glBindBuffer(GL_ARRAY_BUFFER, texBufIDRef);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &(billboardTex[0]), GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

BillboardRenderComponent::~BillboardRenderComponent() {

}

void BillboardRenderComponent::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {
	ShaderManager& shaderManager = ShaderManager::instance();
	const std::shared_ptr<Program> shaderProgram = shaderManager.bindShader(shaderName_);
	billboardTexture_->bind(0, shaderProgram);
	glUniform1i(shaderProgram->getUniform("textureActive"), 1);

	glBindVertexArray(vaoID);
	GLint h_pos = shaderManager.getShaderProgram(shaderName_)->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufIDRef);
	glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);

	GLint h_tex = shaderManager.getShaderProgram(shaderName_)->getAttribute("vertTex");
	if (h_tex != -1 && texBufIDRef != 0) {
		GLSL::enableVertexAttribArray(h_tex);
		glBindBuffer(GL_ARRAY_BUFFER, texBufIDRef);
		glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	}

	shaderManager.renderBillboard(holder_, shaderName_, shape_, material_, billboardTexture_, P, V, M);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 1, 4);
	billboardTexture_->unbind();

	if (h_tex != -1) {
		GLSL::disableVertexAttribArray(h_tex);
	}

	GLSL::disableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shaderManager.unbindShader();
}

void BillboardRenderComponent::renderShadow(std::shared_ptr <MatrixStack> M) {
	// No shadows on billboard effects currently
}
