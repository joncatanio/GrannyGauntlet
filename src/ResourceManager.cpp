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
std::shared_ptr<Shape> ResourceManager::loadShape(const std::string filename, std::vector<std::string> manualTextures) {
	std::shared_ptr<Shape> shape = std::make_shared<Shape>();

	shape->loadMesh(resourceDirectory + filename, manualTextures);
	shape->resize();
	shape->init();

	return shape;
}

std::shared_ptr<Texture> ResourceManager::loadTexture(const std::string relativePathToTexture) {
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->loadTexture(resourceDirectory + relativePathToTexture, relativePathToTexture);

	return texture;
}

ResourceManager::ResourceManager() {
	resourceDirectory = "";
}
