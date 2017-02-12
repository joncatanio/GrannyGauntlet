#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include "ShaderHelper.h"

#include <unordered_map>

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
