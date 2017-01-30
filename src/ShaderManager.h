#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "GLSL.h"
#include "Program.h"

// Manages shaders used by the geometry of the world
class ShaderManager {
public:

	~ShaderManager() {}

	// Returns the single instance of the ShaderManager
	static ShaderManager& instance();

	std::shared_ptr<Program> getShaderProgram(const std::string& shaderProgramName);

	int addVertexShader(const std::string& vertexShaderName, std::shared_ptr<std::string> shaderSource);

	int addFragmentShader(const std::string& fragmentShaderName, std::shared_ptr<std::string> shaderSource);

	void createShaderProgram(const std::string& shaderProgramName, const std::string& vertexShaderName, const std::string& fragmentShaderName);

private:

	std::unordered_map<std::string, GLuint> vertexShaderHandles;

	std::unordered_map<std::string, GLuint> fragmentShaderHandles;

	std::unordered_map<std::string, std::shared_ptr<Program>> shaderPrograms;

	ShaderManager() {}

};

int createAndCompileShader(const std::string& shaderName, std::shared_ptr<std::string> shaderSource, GLenum shaderType);

#endif
