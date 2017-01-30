#include "ResourceManager.h"

ResourceManager& ResourceManager::instance() {
	static ResourceManager *instance = new ResourceManager();
	return *instance;
}

void ResourceManager::setResourceDirectory(const std::string& newResourceDirectory) {
	resourceDirectory = newResourceDirectory;
}

std::shared_ptr<std::string> ResourceManager::loadShader(const std::string& shaderFileName) {
	const char *tempShaderSource = GLSL::textFileRead((resourceDirectory + shaderFileName).c_str());
	std::shared_ptr<std::string> shaderSource = nullptr;

	if (tempShaderSource != NULL) {
		shaderSource = std::make_shared<std::string>(tempShaderSource);
	}

	return shaderSource;
}

ResourceManager::ResourceManager() {
	resourceDirectory = "";
}
