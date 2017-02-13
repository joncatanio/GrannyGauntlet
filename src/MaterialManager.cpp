#include "MaterialManager.h"

MaterialManager& MaterialManager::instance() {
   static MaterialManager *instance = new MaterialManager();
   return *instance;
}

MaterialManager::MaterialManager(){}

MaterialManager::~MaterialManager() {}

void MaterialManager::addMaterial(std::string key, std::shared_ptr<Material> value) {
   std::pair<std::string, std::shared_ptr<Material>> matToAdd(key, value);
   materials.insert(matToAdd);
}

std::shared_ptr<Material> MaterialManager::getMaterial(std::string matName) {
   return materials.at(matName);
}
