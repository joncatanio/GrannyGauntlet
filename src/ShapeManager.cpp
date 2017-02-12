#include "ShapeManager.h"

ShapeManager& ShapeManager::instance() {
   static ShapeManager *instance = new ShapeManager();
   return *instance;
}

ShapeManager::ShapeManager() {

}

ShapeManager::~ShapeManager() {}

int ShapeManager::createShape(ResourceManager& resourceManager,
   std::string name, std::string filename) {
   std::shared_ptr<Shape> shape = resourceManager.loadShape(filename);

   if (shape == nullptr) {
      return 1;
   }

   std::pair<std::string, std::shared_ptr<Shape>> shapeToInsert(name, shape);
   shapes.insert(shapeToInsert);

   return 0;
}

std::shared_ptr<Shape> ShapeManager::getShape(const std::string shapeName) {
   return shapes.at(shapeName);
}
