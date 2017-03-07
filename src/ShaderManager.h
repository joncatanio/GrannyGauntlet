#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "GLSL.h"
#include "Program.h"
#include "ResourceManager.h"

#define MAX_DIRECTIONAL_LIGHTS 10

enum class LightType { POINT, DIRECTIONAL, AREA };

/**
 * Data structure that represents a prescence of light
 *
 * position 	- Represents the position of the light
 * color 		- Represents the color of the light
 * orientation  - Represents the orientation of the light
 * Type 		- The type of light represented
 */
typedef struct Light {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 orientation;
	LightType type;
} Light;

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
	const std::shared_ptr<Program> bindShader(const std::string& shaderProgramName);

	// Unbinds the current shader from use
	void unbindShader();

	// Renders the given object
	void renderObject(std::shared_ptr<GameObject> objToRender, const std::string& shaderName, const std::shared_ptr<Shape> shape,
 	 const std::shared_ptr<Material> material, std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> V, std::shared_ptr<MatrixStack> M);

	// Renders the given object as a billboard
	void renderBillboard(std::shared_ptr<GameObject> objToRender, const std::string& shaderName, const std::shared_ptr<Shape> shape,
		const std::shared_ptr<Material> material, const std::shared_ptr<Texture> billboardTexture, std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> V,
		std::shared_ptr<MatrixStack> M);

	// Render the given object to the shadowmap
	void renderShadowPass(std::shared_ptr<GameObject> objToRender, const std::shared_ptr<Shape> shape,
						  std::shared_ptr<MatrixStack> M);
	// Returns an actual LightType enum value of the given string
	static LightType stringToLightType(std::string type);

    static constexpr const char* shadowPassShaderName = "shadowPass";

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

	// Calculate the View Matrix for the given light (for Shadow Mapping)
	glm::mat4 calculateLightView(std::shared_ptr<Light> light);

	// Calculate the Projection Matrix for the given light (for Shadow Mapping)
	glm::mat4 calculateLightProjection(std::shared_ptr<Light> light);

    // calculate the "middle" of the shoadow map in world sapce
    glm::vec3 calculateShadowMapMid();

    // calculate the light position as used for shadow mapping
    glm::vec3 calculateShadowMapLightPos(std::shared_ptr<Light> light);

	float getViewFrustumMaxDiagonal();

	ShaderManager();

};

// Helper function for shared code used to compile new shaders
GLuint createAndCompileShader(const std::string& shaderName, std::shared_ptr<std::string> shaderSource, GLenum shaderType);

#endif
