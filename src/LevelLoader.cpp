#include "LevelLoader.h"
// Individual component headers
#include "WallRenderComponent.h"
#include "WallPhysicsComponent.h"
#include "PlayerInputComponent.h"
#include "PlayerPhysicsComponent.h"
#include "PlayerRenderComponent.h"
#include "CookieActionComponent.h"
#include "FireHydrantPhysicsComponent.h"
#include "FractureObjectPhysicsComponent.h"
#include "SkyboxRenderComponent.h"

// Manager headers
#include "ResourceManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "ShapeManager.h"
#include "MaterialManager.h"
#include "AudioManager.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

using json = nlohmann::json;

LevelLoader& LevelLoader::instance() {
   static LevelLoader *instance = new LevelLoader();
   return *instance;
}

LevelLoader::LevelLoader() {}

LevelLoader::~LevelLoader() {}


/* Open for reading only for now, if we want to save gamestate we can at
   some point. Possible saveLevel() member function? */
int LevelLoader::loadLevel(GameWorld &world, std::shared_ptr<GameObject> &player) {
   std::ifstream level_file("../levels/grandmas-awakening.json",
      std::ifstream::in);
   int res = 0;

   if (!level_file) {
      std::cerr << "Error reading JSON level file." << std::endl;
      return res;
   }
   json level;
   level_file >> level;

   std::cout << "level_name: " << level["level-name"] << std::endl;

   if ((res = parseShaders(level["level-shaders"]))) {
      std::cerr << "Error parsing isomorphic shaders." << std::endl;
      return res;
   }

   if ((res = parseShapes(level["level-shapes"]))) {
      std::cerr << "Error parsing shapes." << std::endl;
      return res;
   }

   if ((res = parseTextures(level["level-textures"]))) {
      std::cerr << "Error parsing textures." << std::endl;
      return res;
   }

   if ((res = parseBillboards(level["billboards"]))) {
	   std::cerr << "Error parsing billboards." << std::endl;
	   return res;
   }

   if ((res = parseMaterials(level["level-materials"]))) {
      std::cerr << "Error parsing materials." << std::endl;
      return res;
   }

   if ((res = parseCharacters(world, player, level["characters"]))) {
      std::cerr << "Error parsing characters." << std::endl;
      return res;
   }

   if ((res = parseStaticObjects(world, level["static-objects"]))) {
      std::cerr << "Error parsing static objects." << std::endl;
      return res;
   }

   if ((res = parseDynamicObjects(world, level["dynamic-objects"]))) {
      std::cerr << "Error parsing dynamic objects." << std::endl;
      return res;
   }

   if ((res = parseLights(world, level["lights"]))) {
      std::cerr << "Error parsing lights." << std::endl;
      return res;
   }

   if ((res = parseSoundtrack(level["soundtrack"]))) {
      std::cerr << "Error parsing soundtrack." << std::endl;
      return res;
   }

   if ((res = parseHellSoundtrack(level["hell-soundtrack"]))) {
      std::cerr << "Error parsing hell-soundtrack." << std::endl;
      return res;
   }

   if ((res = parseSoundeffects(level["soundeffects"]))) {
      std::cerr << "Error parsing soundeffects." << std::endl;
      return res;
   }

   return res;
}

int LevelLoader::parseShaders(json shaders) {
   ResourceManager& resourceManager = ResourceManager::instance();

   if (shaders != nullptr) {
      ShaderManager& shaderManager = ShaderManager::instance();

      for (json shader : shaders) {
         std::string shaderName = shader["name"];

         if (shaderManager.createIsomorphicShader(resourceManager,
             shaderName, shader["file-prefix"]) == 0) {
            return 1;
         }
         if (shader["default"]) {
             shaderManager.setDefaultShader(shaderName);
         }
      }
      // load shadow pass shader
      if (shaderManager.createIsomorphicShader(resourceManager, ShaderManager::shadowPassShaderName,
                                               ShaderManager::shadowPassShaderName) == 0) {
         return 1;
      }
   }

   return 0;
}

int LevelLoader::parseShapes(json shapes) {
   ResourceManager& resourceManager = ResourceManager::instance();

   if (shapes != nullptr) {
      ShapeManager& shapeManager = ShapeManager::instance();

       std::vector<std::string> empty;
      for (json shape : shapes) {
          std::vector<std::string> manualTextures;
          if(shape["manual_texture"] == nullptr) {
              if (shapeManager.createShape(resourceManager, shape["name"],
                                           shape["filename"], empty)) {
                  return 1;
              }
          } else {

              for (std::string texName : shape["manual_texture"]) {
                  manualTextures.push_back(texName);
              }
              if (shapeManager.createShape(resourceManager, shape["name"],
                                           shape["filename"], manualTextures)) {
                  return 1;
              }

          }
      }


      if (shapeManager.createShape(resourceManager, "Arrow", "arrow.obj", empty)) {
          return 1;
      }
   }

   return 0;
}

int LevelLoader::parseTextures(json textures) {
   ResourceManager& resourceManager = ResourceManager::instance();

   if (textures != nullptr) {
      TextureManager &textureManager = TextureManager::instance();

      for(json texture : textures) {
         if (textureManager.createTexture(resourceManager, texture["name"], texture["filename"])) {
            return 1;
         }
      }

      return 0;

   }
}

int LevelLoader::parseBillboards(json billboards) {
	ResourceManager& resourceManager = ResourceManager::instance();

	if (billboards != nullptr) {
		ShaderManager& shaderManager = ShaderManager::instance();

		for (json billboard : billboards) {
			if (billboard["name"] == nullptr || billboard["texture"] == nullptr || billboard["random"] == nullptr) {
				return 1;
			}

			std::shared_ptr<Texture> billboardTexture = resourceManager.loadTexture(billboard["texture"]);
			shaderManager.addNewBillboard(billboard["name"], billboardTexture, billboard["random"]);
		}
	}

	return 0;
}

int LevelLoader::parseMaterials(json materials) {
   if (materials != nullptr) {
      MaterialManager& materialManager = MaterialManager::instance();

      for (json mat : materials) {
         std::shared_ptr<Material> material = std::make_shared<Material>();
         *material = {
            mat["ambient"]["r"], mat["ambient"]["g"], mat["ambient"]["b"],
            mat["diffuse"]["r"], mat["diffuse"]["g"], mat["diffuse"]["b"],
            mat["specular"]["r"], mat["specular"]["g"], mat["specular"]["b"],
            mat["shininess"]
         };
          if(mat["alpha"] == nullptr){
              material->alpha = 1.0;
          } else {
              material->alpha = mat["alpha"];
          }

         materialManager.addMaterial(mat["name"], material);
      }
   }

   return 0;
}

/* Later make a map of available players so they can swap out to a new
   main character whenever they want. */
int LevelLoader::parseCharacters(GameWorld &world,
   std::shared_ptr<GameObject> &player, json chars) {

   for (json character : chars) {
      if (character["yAxis-rotation-deg"] == nullptr ||
          character["orient-angle-deg"] == nullptr) {
         return 1;
      }

      /* Set the orient angle to orient the object correctly from it's starting pos.
       * This is specific to each obj file. Positive values are cw, negative ccw */
      float yRotRad = static_cast<float>(character["yAxis-rotation-deg"]) * M_PI / 180.0f;
      float orientRad = static_cast<float>(character["orient-angle-deg"]) * M_PI / 180.0f;

      player = createGameObject(character, GameObjectType::PLAYER);

      player->initComponents();
      player->setYAxisRotation(yRotRad);
      player->setOrientAngle(orientRad);

      // TODO (noj) when we have multiple chars only add the main char
      world.addDynamicGameObject(player);
   }

   return 0;
}

int LevelLoader::parseStaticObjects(GameWorld &world, json staticObjs) {
   if (staticObjs != nullptr) {
      for (json gameObj : staticObjs) {
          std::shared_ptr<GameObject> staticGameObj;
          if (gameObj["object-type"] == "SKY_BOX") {
              staticGameObj = createGameObject(gameObj, GameObjectType::SKY_BOX);
          } else if(gameObj["object-type"] == "HELL_BOX") {
              staticGameObj = createGameObject(gameObj, GameObjectType::HELL_BOX);
          } else if(gameObj["object-type"] == "PORTAL_TO_HELL") {
              staticGameObj = createGameObject(gameObj, GameObjectType::PORTAL_TO_HELL);
          }else {
            staticGameObj = createGameObject(gameObj, GameObjectType::STATIC_OBJECT); 
         }

         staticGameObj->initComponents();
         if (gameObj["yAxis-rotation-deg"] != nullptr) {
            float yRotRad = static_cast<float>(gameObj["yAxis-rotation-deg"]) * M_PI / 180.0f;
            staticGameObj->setYAxisRotation(yRotRad);
         }
		 if (gameObj["orient-angle-deg"] != nullptr) {
			 float orientRad = static_cast<float>(gameObj["orient-angle-deg"]) * M_PI / 180.0f;
			 staticGameObj->setOrientAngle(orientRad);
		 }

         if (gameObj["object-type"] == "SKY_BOX") {
            world.setSkybox(staticGameObj);
         } else if(gameObj["object-type"] == "HELL_BOX") {
             world.setHellbox(staticGameObj);
         } else {
            world.addStaticGameObject(staticGameObj);
         }
      }
   }



   return 0;
}

int LevelLoader::parseDynamicObjects(GameWorld &world, json dynamicObjs) {
   if (dynamicObjs != nullptr) {
      for (json gameObj : dynamicObjs) {
         std::shared_ptr<GameObject> dynamicGameObj =
            createGameObject(gameObj, GameObjectType::DYNAMIC_OBJECT); 

         dynamicGameObj->initComponents();
		 if (gameObj["yAxis-rotation-deg"] != nullptr) {
			 float yRotRad = static_cast<float>(gameObj["yAxis-rotation-deg"]) * M_PI / 180.0f;
			 dynamicGameObj->setYAxisRotation(yRotRad);
		 }
		 if (gameObj["orient-angle-deg"] != nullptr) {
			 float orientRad = static_cast<float>(gameObj["orient-angle-deg"]) * M_PI / 180.0f;
			 dynamicGameObj->setOrientAngle(orientRad);
		 }

         world.addDynamicGameObject(dynamicGameObj);
      }
   }

   return 0;
}

int LevelLoader::parseLights(GameWorld &world, json lightObjs) {
   if (lightObjs != nullptr) {
      for (json lightObj : lightObjs) {
         std::shared_ptr<Light> light = std::make_shared<Light>();
         *light = {
            glm::vec3(lightObj["pos"]["x"], lightObj["pos"]["y"], lightObj["pos"]["z"]),
            glm::vec3(lightObj["color"]["r"], lightObj["color"]["g"], lightObj["color"]["b"]),
            glm::vec3(lightObj["orientation"]["x"], lightObj["orientation"]["y"], lightObj["orientation"]["z"]),
            ShaderManager::stringToLightType(lightObj["light-type"])
         };

         world.addLight(light);
      }
   }

   return 0;
}

int LevelLoader::parseSoundtrack(json soundtrackObjs) {
   AudioManager& audioManager = AudioManager::instance();

   if (soundtrackObjs != nullptr) {
      for (json track : soundtrackObjs) {
         audioManager.addTrack(track["filename"]);
      }
   }

   return 0;
}

int LevelLoader::parseHellSoundtrack(json soundtrackObjs) {
   AudioManager& audioManager = AudioManager::instance();

   if (soundtrackObjs != nullptr) {
      for (json track : soundtrackObjs) {
         audioManager.addAltTrack(track["filename"]);
      }
   }

   return 0;
}

int LevelLoader::parseSoundeffects(json soundeffectObjs) {
   AudioManager& audioManager = AudioManager::instance();

   if (soundeffectObjs != nullptr) {
      for (json effect : soundeffectObjs) {
         audioManager.addEffect(effect["name"], effect["filename"]);
      }
   }

   return 0;
}

std::shared_ptr<GameObject> LevelLoader::createGameObject(json obj,
   GameObjectType objType) {
   InputComponent* inputComponent = getInputComponent(obj);
   PhysicsComponent* physicsComponent = getPhysicsComponent(obj);
   RenderComponent* renderComponent = getRenderComponent(obj);
   ActionComponent* actionComponent = getActionComponent(obj);
   
   std::shared_ptr<GameObject> gameObj = std::make_shared<GameObject>(
      GameObject::stringToType(obj["object-type"]),
      glm::vec3(obj["pos"]["x"], obj["pos"]["y"], obj["pos"]["z"]),
      glm::vec3(obj["dir"]["x"], obj["dir"]["y"], obj["dir"]["z"]),
      static_cast<float>(obj["vel"]),
      glm::vec3(obj["scale"]["x"], obj["scale"]["y"], obj["scale"]["z"]),
      inputComponent,
      physicsComponent,
      renderComponent,
      actionComponent,
      obj["deliverable"]
   );

   return gameObj;
}

InputComponent* LevelLoader::getInputComponent(json obj) {
   if (obj["input-component"] == nullptr) {
      return nullptr;
   }

   std::string componentName = obj["input-component"]["name"];

   if (componentName == "PlayerInputComponent") {
      return new PlayerInputComponent();
   }

   return nullptr;
}

PhysicsComponent* LevelLoader::getPhysicsComponent(json obj) {
   // No physics component included
   if (obj["physics-component"] == nullptr) {
      return nullptr;
   }

   // Invalid physics component syntax
   if (obj["physics-component"]["name"] == nullptr) {
      std::cerr << "Warning - Invalid physics component syntax" << std::endl;
      return nullptr;
   }

   std::string componentName = obj["physics-component"]["name"];

   if (componentName == "WallPhysicsComponent") {
      return new WallPhysicsComponent();
   } else if (componentName == "PlayerPhysicsComponent") {
      return new PlayerPhysicsComponent();
   } else if (componentName == "FireHydrantPhysicsComponent") {
      return new FireHydrantPhysicsComponent();
   } else if (componentName == "FractureObjectPhysicsComponent") {
	  return new FractureObjectPhysicsComponent();
   }

   return nullptr;
}

RenderComponent* LevelLoader::getRenderComponent(json obj) {
    ShapeManager &shapeManager = ShapeManager::instance();
    MaterialManager &materialManager = MaterialManager::instance();

    // No render component included
    if (obj["render-component"] == nullptr) {
        return nullptr;
    }

    // Invalid render component syntax
    if (obj["render-component"]["name"] == nullptr ||
        obj["render-component"]["shape"] == nullptr ||
        obj["render-component"]["shader"] == nullptr ||
        obj["render-component"]["material"] == nullptr) {
        std::cerr << "Warning - Invalid render component syntax" << std::endl;
        return nullptr;
    }

    std::string componentName = obj["render-component"]["name"];
    std::string shapeName = obj["render-component"]["shape"];
    std::string shaderName = obj["render-component"]["shader"];
    std::string materialName = obj["render-component"]["material"];

    if (componentName == "WallRenderComponent") {
        return new WallRenderComponent(shapeManager.getShape(shapeName),
                                       shaderName, materialManager.getMaterial(materialName));
    } else if (componentName == "PlayerRenderComponent") {
        return new PlayerRenderComponent(shapeManager.getShape(shapeName),
                                         shaderName, materialManager.getMaterial(materialName));
    } else if (componentName == "SkyboxRenderComponent") {
       if (obj["render-component"]["skybox-path"] == nullptr ||
           obj["render-component"]["file-extension"] == nullptr) {
          return nullptr;
       }
        return new SkyboxRenderComponent(shapeManager.getShape(shapeName),
                                         shaderName, materialManager.getMaterial(materialName),
                                         obj["render-component"]["skybox-path"],
                                         obj["render-component"]["file-extension"]);
    } else if (componentName == "FireHydrantRenderComponent") {
      return nullptr;
   }

   return nullptr;
}

ActionComponent* LevelLoader::getActionComponent(json obj) {
   if (obj["action-component"] == nullptr) {
      return nullptr;
   }

   std::string componentName = obj["action-component"]["name"];

   if (componentName == "CookieActionComponent") {
      return new CookieActionComponent();
   }

   return nullptr;
}

