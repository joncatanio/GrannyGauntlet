#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <unordered_map>

// TODO(rgarmsen2295): Change to glm::vec3 for rgb variables
typedef struct {
	GLfloat rAmb, gAmb, bAmb;
	GLfloat rDif, gDif, bDif;
	GLfloat rSpc, gSpc, bSpc;
	GLfloat shininess;
	GLfloat alpha;
} Material;

class MaterialManager {
public:
   static MaterialManager& instance();

   ~MaterialManager();

   void addMaterial(std::string key, std::shared_ptr<Material> value);

   std::shared_ptr<Material> getMaterial(std::string matName);
private:
   MaterialManager();

   // Key is the material name defined in json file
   std::unordered_map<std::string, std::shared_ptr<Material>> materials;
};

#endif
