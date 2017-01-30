#include "ShaderManager.h"

ShaderManager& ShaderManager::instance() {
	static ShaderManager *instance = new ShaderManager();
	return *instance;
}

const std::string& ShaderManager::getBoundShaderName() {
	return boundShaderName;
}

std::shared_ptr<Program> ShaderManager::getShaderProgram(const std::string& shaderProgramName) {
	return shaderPrograms.at(shaderProgramName);
}

GLuint ShaderManager::createVertexShader(const std::string& vertexShaderName, std::shared_ptr<std::string> shaderSource) {
	
	// Compile the shader and check for errors
	GLuint vertexShaderHandle = createAndCompileShader(vertexShaderName, shaderSource, GL_VERTEX_SHADER);
	if (vertexShaderHandle == 0) {
		return -1;
	}

	// Add the new vertex shader to the hash map
	std::pair<std::string, GLuint> newVertexShader(vertexShaderName, vertexShaderHandle);
	vertexShaderHandles.insert(newVertexShader);

	return vertexShaderHandle;
}

GLuint ShaderManager::createFragmentShader(const std::string& fragmentShaderName, std::shared_ptr<std::string> shaderSource) {
	
	// Compile the shader and check for errors
	GLuint fragmentShaderHandle = createAndCompileShader(fragmentShaderName, shaderSource, GL_FRAGMENT_SHADER);
	if (fragmentShaderHandle == 0) {
		return -1;
	}

	// Add the new fragment shader to the hash map
	std::pair<std::string, GLuint> newFragmentShader(fragmentShaderName, fragmentShaderHandle);
	fragmentShaderHandles.insert(newFragmentShader);

	return fragmentShaderHandle;
}

GLuint createAndCompileShader(const std::string& shaderName, std::shared_ptr<std::string> shaderSource, GLenum shaderType) {

	// Compile shader and get respective handle ID
	GLuint shaderHandle = glCreateShader(shaderType);

	// Read and compile the shader source data
	const char* shaderSourcePointer = shaderSource->c_str();
	glShaderSource(shaderHandle, 1, &shaderSourcePointer, NULL);
	glCompileShader(shaderHandle);

	// Check compilation success
	GLint compileStatus;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileStatus);
	if(!compileStatus) {
		GLSL::printShaderInfoLog(shaderHandle);

		// Give human readable string for error
		std::string shaderTypeName = "";
		if (shaderType == GL_VERTEX_SHADER) {
			shaderTypeName = "vertex shader";
		} 
		else if (shaderType == GL_FRAGMENT_SHADER) {
			shaderTypeName = "fragment shader";
		}

		std::cout << "Error compiling " << shaderTypeName << " " << shaderName << std::endl;

		return 0;
	}

	GLSL::printError();
	assert(glGetError() == GL_NO_ERROR);

	return shaderHandle;
}

GLuint ShaderManager::createShaderProgram(const std::string& shaderProgramName, const std::string& vertexShaderName, const std::string& fragmentShaderName) {
	
	// Create the program and link
	GLuint pid = glCreateProgram();
	glAttachShader(pid, vertexShaderHandles[vertexShaderName]);
	glAttachShader(pid, fragmentShaderHandles[fragmentShaderName]);

	// Check link success
	GLint linkStatus;
	glLinkProgram(pid);
	glGetProgramiv(pid, GL_LINK_STATUS, &linkStatus);
	if(!linkStatus) {
		GLSL::printProgramInfoLog(pid);
		std::cout << "Error linking shaders " << vertexShaderName << " and " << fragmentShaderName << " for shader program " << shaderProgramName << std::endl;

		return 0;
	}
	
	GLSL::printError();
	assert(glGetError() == GL_NO_ERROR);

	// Instantiate the actual program class using the given program ID and add it to the hash map
	std::shared_ptr<Program> shaderProgram = std::make_shared<Program>(shaderProgramName, vertexShaderName, fragmentShaderName, pid);
	std::pair<std::string, std::shared_ptr<Program>> newShaderProgram(shaderProgramName, shaderProgram);
	shaderPrograms.insert(newShaderProgram);

	return pid;
}

void ShaderManager::bindShader(const std::string& shaderProgramName) {
	std::shared_ptr<Program>& shaderToBind = shaderPrograms.at(shaderProgramName);
	boundShaderName = shaderProgramName;
	glUseProgram(shaderToBind->getPid());
}

void ShaderManager::unbindShader() {
	boundShaderName = "";
	glUseProgram(NO_SHADER);
}

ShaderManager::ShaderManager() {
	boundShaderName = "";
}
