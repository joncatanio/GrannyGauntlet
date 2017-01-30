#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "GLSL.h"
#include "Program.h"
#include "ResourceManager.h"

// Manages shaders used by the geometry of the world
class ShaderManager {
public:

	~ShaderManager() {}

	// Returns the single instance of the ShaderManager
	static ShaderManager& instance();

	// Returns the currently bound shader program name
	const std::string& getBoundShaderName();

	// Gets a shared pointer to the shader program with the given name.
	// Throws an |out_of_range| exception if no shader program with that name is found
	std::shared_ptr<Program> getShaderProgram(const std::string& shaderProgramName);

	// Creates a vertex shader from the passed source string with the given name.
	// Returns the vertex handle on success, 0 on failure
	GLuint createVertexShader(const std::string& vertexShaderName, std::shared_ptr<std::string> shaderSource);

	// Creates a fragment shader from the passed source string with the given name.
	// Returns the fragment handle on success, 0 on failure
	GLuint createFragmentShader(const std::string& fragmentShaderName, std::shared_ptr<std::string> shaderSource);

	// Creates a shader program by linking a compiled vertex and fragment shader.
	// Returns the program ID on success, 0 on failure
	GLuint createShaderProgram(const std::string& shaderProgramName, const std::string& vertexShaderName, const std::string& fragmentShaderName);

	// Builds a shader program under the assumption that all parts of the shader (vertex, fragment, program) will have the same name.
	// Also, each shader resource file should have the same prefix (e.g. |shaderResourcePrefix| + "_frag.glsl").
	// Returns the program ID on success, 0 on failure
	GLuint createIsomorphicShader(ResourceManager& resourceManager, const std::string& shaderName, const std::string& shaderResourcePrefix);

	// Finds the shader program with the given name and binds it.
	// Throws an |out_of_range| exception if no shader program with that name is found
	void bindShader(const std::string& shaderProgramName);

	// Unbinds the current shader from use
	void unbindShader();

private:

	// Special program ID that represents a no/null program shader
	const GLuint NO_SHADER = 0;

	// The currently bound shader program name.  If none is bound, then it is set to the empty string
	std::string boundShaderName;

	// A hash map of the currently compiled vertex shaders. The key is the shader name and the value is the handle
	std::unordered_map<std::string, GLuint> vertexShaderHandles;

	// A hash map of the currently compiled fragment shaders. The key is the shader name and the value is the handle
	std::unordered_map<std::string, GLuint> fragmentShaderHandles;

	// A hash map of the currently linked shader programs. The key is the shader program name and the value is a pointer to the Program
	std::unordered_map<std::string, std::shared_ptr<Program>> shaderPrograms;

	ShaderManager();

};

// Helper function for shared code used to compile new shaders
GLuint createAndCompileShader(const std::string& shaderName, std::shared_ptr<std::string> shaderSource, GLenum shaderType);

#endif
