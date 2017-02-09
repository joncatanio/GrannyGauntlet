#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <memory>

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

private:

	// The directory to parse resources from
	std::string resourceDirectory;

	ResourceManager();

};

#endif
