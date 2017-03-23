#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "GameWorld.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "ActionComponent.h"
#include "json.hpp"

class LevelLoader {
public:
   static LevelLoader& instance();

   ~LevelLoader();

   // Loads a level and populates reference of GameWorld, returns 0 on success
   int loadLevel(GameWorld &world, std::shared_ptr<GameObject> &player);
private:
   LevelLoader();

   int parseShaders(nlohmann::json shaders);
   int parseShapes(nlohmann::json shapes);
    int parseTextures(nlohmann::json textures);
   int parseBillboards(nlohmann::json billboards);
   int parseMaterials(nlohmann::json materials);
   int parseCharacters(GameWorld &world, std::shared_ptr<GameObject> &player,
      nlohmann::json chars);
   int parseStaticObjects(GameWorld &world, nlohmann::json staticObjs);
   int parseDynamicObjects(GameWorld &world, nlohmann::json dynamicObjs);
   int parseLights(GameWorld &world, nlohmann::json lightObjs);
   int parseSoundtrack(nlohmann::json soundtrackObjs);
   int parseHellSoundtrack(nlohmann::json soundtrackObjs);
   int parseSoundeffects(nlohmann::json soundeffectObjs);
   std::shared_ptr<GameObject> createGameObject(nlohmann::json obj,
      GameObjectType objType);

   InputComponent* getInputComponent(nlohmann::json obj);
   PhysicsComponent* getPhysicsComponent(nlohmann::json obj);
   RenderComponent* getRenderComponent(nlohmann::json obj);
   ActionComponent* getActionComponent(nlohmann::json obj);
};

#endif
