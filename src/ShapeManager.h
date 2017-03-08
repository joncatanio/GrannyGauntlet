#ifndef SHAPE_MANAGER_H
#define SHAPE_MANAGER_H

#include "ResourceManager.h"
#include "Shape.h"

#include <unordered_map>

class ShapeManager {
public:
   static ShapeManager& instance();

   ~ShapeManager();

   // Loads the shape using the ResourceManager and adds it to the map
   int createShape(ResourceManager& resourceManager, std::string name,
      std::string filename, std::string manualTexture = "");

   std::shared_ptr<Shape> getShape(const std::string shapeName);
private:
   ShapeManager();

   // Key is the shape name defined in json file
   std::unordered_map<std::string, std::shared_ptr<Shape>> shapes;
};

#endif
