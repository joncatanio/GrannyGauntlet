#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "ResourceManager.h"
#include "Texture.h"

#include <unordered_map>

class TextureManager {
public:
    static TextureManager& instance();

    ~TextureManager();

    int createTexture(ResourceManager& resourceManager, std::string name,
                    std::string filename);

    std::shared_ptr<Texture> getTexture(const std::string textureName);

    void increaseTextureToggle(){textureToggle++;}
    int getTextureToggle(){ return textureToggle;}
private:
    TextureManager();

    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

    int textureToggle;
};

#endif
