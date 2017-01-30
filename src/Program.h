#ifndef PROGRAM_H
#define PROGRAM_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <unordered_map>
#include <string>

// Represents a shader program used to shade objects in OpenGL
class Program {
public:

	// The name associated with the shader program
	const std::string name;

	Program(const std::string shaderProgramName, const std::string vertexShaderName, const std::string fragmentShaderName, const GLuint pid);

	virtual ~Program();

	// Adds a new attribute associated with the shader program
	void addAttribute(const std::string &name);

	// Adds a new uniform associated with the shader program
	void addUniform(const std::string &name);

	// Returns the ID of this shader program
	GLuint getPid();

	// Returns the location of the attribute with the given name.
	// Throws an |out_of_range| exception if no attribute with the given name is found
	GLint getAttribute(const std::string &name) const;

	// Returns the location of the attribute with the given name.
	// Throws an |out_of_range| exception if no uniform with the given name is found
	GLint getUniform(const std::string &name) const;
	
protected:
	
private:

	// The name associated with the linked vertex shader
	const std::string vertexShaderName;

	// The name associated with the linked fragment  shader
	const std::string fragmentShaderName;

	// The ID of the shader program
	const GLuint pid;

	// A hash map of attribute names associated with the program and their OpenGL location
	std::unordered_map<std::string, GLint> attributes;

	// A hash map of uniform names associated with the program and their OpenGL location
	std::unordered_map<std::string, GLint> uniforms;

	// Adds default attributes and uniforms common to most useful shaders
	void addDefaultAttributesAndUniforms();
};

#endif
