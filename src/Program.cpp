#include "Program.h"

#include <iostream>
#include <cassert>

#include "GLSL.h"

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

	// Adds light uniforms
	addUniform("lightPos");
	addUniform("lightClr");

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
