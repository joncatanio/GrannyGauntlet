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

void Program::addDefaultAttributesAndUniforms() {

	// Adds transform uniforms
	addUniform("M");
	addUniform("V");
	addUniform("P");

	// Add light View and Projection
	addUniform("lightV");
	addUniform("lightP");

	// Add shadow map size
	addUniform("shadowMapSize");

	// Add transpose inverse M, so no per vertex calculation
	addUniform("tiM");

	// Add directional light uniforms
	addUniform("numDirectionLights");
	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i) {
		addUniform("directionLights[" + std::to_string(i) + "].position");
		addUniform("directionLights[" + std::to_string(i) + "].color");
		addUniform("directionLights[" + std::to_string(i) + "].orientation");
	}

	//TODO(nurgan) remove from default attributs
	addUniform("cubemap");

	// TODO(rgarmsen2295): Remove from default attributes
	addUniform("billboardTransform");

	// I'd give this a TODO to remove but I don't think this is gonna change at this point
	addUniform("colorScale");

	addUniform("hellMode");

	addUniform("shadowMapTex");
	addUniform("textureMap");

    addUniform("textureActive");

	// Adds material uniforms
	addUniform("MatAmb");
	addUniform("MatDif");
	addUniform("MatSpc");
	addUniform("MatShiny");
	addUniform("MatAlpha");

	// Adds vertex position and normal attributes
	addAttribute("vertPos");
	addAttribute("vertNor");
	addAttribute("vertTex");

	//TODO(nurgan) remove from default
	addAttribute("lifePercent");
}
