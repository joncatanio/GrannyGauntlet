#include "ShaderManager.h"
#include "ShapeManager.h"

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

GLuint ShaderManager::createIsomorphicShader(ResourceManager& resourceManager, const std::string& shaderName, const std::string& shaderResourcePrefix) {
	if (createVertexShader(shaderName, resourceManager.loadShader(shaderResourcePrefix + "_vert.glsl")) == 0) {
		return 0;
	}

	if (createFragmentShader(shaderName, resourceManager.loadShader(shaderResourcePrefix + "_frag.glsl")) == 0) {
		return 0;
	}

	return createShaderProgram(shaderName, shaderName, shaderName);
}

const std::shared_ptr<Program> ShaderManager::bindShader(const std::string& shaderProgramName) {
	std::shared_ptr<Program> shaderToBind = shaderPrograms.at(shaderProgramName);
	boundShaderName = shaderProgramName;
	glUseProgram(shaderToBind->getPid());

    shaderToBind->bindTextures();

	return shaderToBind;
}

void ShaderManager::unbindShader() {
	shaderPrograms.at(boundShaderName)->unbindTextures();
    boundShaderName = "";
	glUseProgram(NO_SHADER);
}

void ShaderManager::renderObject(std::shared_ptr<GameObject> objToRender, const std::string& shaderName, const std::shared_ptr<Shape> shape,
 const std::shared_ptr<Material> material, std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> V, std::shared_ptr<MatrixStack> M) {
	if (objToRender != NULL) {
		const std::shared_ptr<Program> shaderProgram = bindShader(shaderName);

		// Bind perspective and view tranforms
		glUniformMatrix4fv(shaderProgram->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(shaderProgram->getUniform("V"), 1, GL_FALSE, glm::value_ptr(V->topMatrix()));

		// Set up lights
		GameManager& gameManager = GameManager::instance();
		GameWorld& gameWorld = gameManager.getGameWorld();

		// Point lights
		// TODO(rgarmsen2295): Add point lights back

		// Directional lights
		// TODO(rgarmsen2295): Implement more cleanly using "uniform buffer objects"
		const std::vector<std::shared_ptr<Light>>& directionalLights = gameWorld.getDirectionalLights();
		int numDirectionLights = directionalLights.size();
		
		glUniform1i(shaderProgram->getUniform("numDirectionLights"), numDirectionLights);
		for (int i = 0; i < numDirectionLights; ++i) {
			const std::shared_ptr<Light> light = directionalLights.at(i);
			glUniform3f(shaderProgram->getUniform("directionLights[" + std::to_string(i) + "].position"), light->position.x, light->position.y, light->position.z);
			glUniform3f(shaderProgram->getUniform("directionLights[" + std::to_string(i) + "].color"), light->color.x, light->color.y, light->color.z);
			glUniform3f(shaderProgram->getUniform("directionLights[" + std::to_string(i) + "].orientation"), light->orientation.x, light->orientation.y, light->orientation.z);
		}

		// Bind material properties
		glUniform3f(shaderProgram->getUniform("MatAmb"), material->rAmb, material->gAmb, material->bAmb);
		glUniform3f(shaderProgram->getUniform("MatDif"), material->rDif, material->gDif, material->bDif);
		glUniform3f(shaderProgram->getUniform("MatSpc"), material->rSpc, material->gSpc, material->bSpc);
		glUniform1f(shaderProgram->getUniform("MatShiny"), material->shininess);

		// Set up and bind model transform
		M->pushMatrix();
		M->loadIdentity();

		M->translate(objToRender->getPosition());
		M->scale(objToRender->getScale());

      glm::mat4 rotation = objToRender->transform.getRotate();
		M->rotateMat4(rotation);

		glUniformMatrix4fv(shaderProgram->getUniform("M"), 1, GL_FALSE, glm::value_ptr(M->topMatrix()));

		glm::mat4 tiM = glm::transpose(glm::inverse(M->topMatrix()));
		glUniformMatrix4fv(shaderProgram->getUniform("tiM"), 1, GL_FALSE, glm::value_ptr(tiM));

		// Draw bunny
		// TODO(rgarmsen): Make shape not need the shader program
		shape->draw(shaderProgram);

		M->popMatrix();


#ifdef DEBUG_BB
      ShapeManager& shapeManager = ShapeManager::instance();
      std::shared_ptr<Shape> shapeToDraw = shapeManager.getShape("Sphere");
      if (objToRender->getBoundingBox()) {
         for (int i = 0; i < 8; i++) {
            M->pushMatrix();
            M->loadIdentity();
            glm::vec3 rotation = glm::vec3(objToRender->getBoundingBox()->boxPoints[i].x,
                      objToRender->getBoundingBox()->boxPoints[i].y,
                      objToRender->getBoundingBox()->boxPoints[i].z
            );

            M->translate(rotation);
		      M->scale(0.25);
            glUniformMatrix4fv(shaderProgram->getUniform("M"), 1, GL_FALSE, glm::value_ptr(M->topMatrix()));

            shapeToDraw->draw(shaderProgram);
            M->popMatrix();
         }
      }
#endif

		unbindShader();
	}
}

LightType ShaderManager::stringToLightType(std::string type) {
	if (type == "POINT") {
		return LightType::POINT;
	} else if(type == "AREA") {
		return LightType::AREA;
	} else {
		//default to directional light
		return LightType::DIRECTIONAL;
	}
}

ShaderManager::ShaderManager() {
	boundShaderName = "";
}
