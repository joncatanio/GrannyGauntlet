#include "Program.h"

#include <iostream>
#include <cassert>
#include <string>

#include "GLSL.h"
#include "ShaderManager.h"

Program::Program(const std::string shaderProgramName, const std::string vertexShaderName, const std::string fragmentShaderName, const GLuint pid)
	: name(shaderProgramName),
	vertexShaderName(vertexShaderName),
	fragmentShaderName(fragmentShaderName),
	pid(pid) {
	addDefaultAttributesAndUniforms();
}

Program::~Program() {
	
}

void Program::addAttribute(const std::string& name) {
	attributes[name] = GLSL::getAttribLocation(pid, name.c_str(), true);
}

void Program::addUniform(const std::string& name) {
	uniforms[name] = GLSL::getUniformLocation(pid, name.c_str(),true);
}

void Program::addTexture(Texture* texture) {
	GLint handle = GLSL::getUniformLocation(pid, texture->name.c_str());
	texture->setHandle(handle);
	textures[name] = texture;
}

GLuint Program::getPid() {
	return pid;
}

GLint Program::getAttribute(const std::string& name) const {
	GLint attributeLocation = attributes.at(name);
	return attributeLocation;
}

GLint Program::getUniform(const std::string& name) const {
	GLint uniformLocation = uniforms.at(name);
	return uniformLocation;
}

Texture* Program::getTexture(const std::string &name) const {
	Texture *texture = textures.at(name);
	return texture;
}

void Program::bindTextures() {
    int unit = 0;
    for (auto it : textures) {
        it.second->bind(unit++);
    }
}

void Program::unbindTextures() {
    for (auto it : textures) {
        it.second->unbind();
    }
}

void Program::addDefaultAttributesAndUniforms() {

	// Adds transform uniforms
	addUniform("M");
	addUniform("V");
	addUniform("P");

	// Add transpose inverse MV, so no per vertex calculation
	addUniform("tiMV");
	
	// Add directional light uniforms
	addUniform("numDirectionLights");
	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i) {
		addUniform("directionLights[" + std::to_string(i) + "].position");
		addUniform("directionLights[" + std::to_string(i) + "].color");
		addUniform("directionLights[" + std::to_string(i) + "].orientation");
	}

	//TODO(nurgan) remove from default attributs
	addUniform("cubemap");

	// Adds material uniforms
	addUniform("MatAmb");
	addUniform("MatDif");
	addUniform("MatSpc");
	addUniform("MatShiny");

	// Adds vertex position and normal uniforms
	addAttribute("vertPos");
	addAttribute("vertNor");
	addAttribute("vertTex");
}
