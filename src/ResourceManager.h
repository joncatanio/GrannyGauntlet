#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <memory>
#include "Shape.h"

#include "GLSL.h"

class ResourceManager {

public:

	~ResourceManager() {}

	// Returns the single instance of the ResourceManager
	static ResourceManager& instance();

	// Sets the root resource directory (can be relative to the executable or fully defined)
	void setResourceDirectory(const std::string& newResourceDirectory);

	// Reads shader source from a file and returns a pointer to the in-memory string
	std::shared_ptr<std::string> loadShader(const std::string& shaderFileName);

   // Loads, resizes, and initializes shape then returns it.
   std::shared_ptr<Shape> loadShape(const std::string filename);
	std::shared_ptr<Shape> loadShape(const std::string filename, const std::string manualTexture);

	std::shared_ptr<Texture> loadTexture(const std::string relativePathToTexture);

private:

	// The directory to parse resources from
	std::string resourceDirectory;

	ResourceManager();

};

#endif
