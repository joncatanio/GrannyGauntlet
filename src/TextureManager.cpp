#include "TextureManager.h"

TextureManager& TextureManager::instance() {
    static TextureManager *instance = new TextureManager();
    return *instance;
}

TextureManager::TextureManager() {

}

TextureManager::~TextureManager() {}

int TextureManager::createTexture(ResourceManager& resourceManager, std::string name, std::string filename) {
    std::shared_ptr <Texture> texture;
    texture = resourceManager.loadTexture(filename);

    if (texture == nullptr) {
        return 1;
    }

    std::pair<std::string, std::shared_ptr<Texture>> textureToInsert(name, texture);
    textures.insert(textureToInsert);

    return 0;
}

std::shared_ptr<Texture> TextureManager::getTexture(const std::string textureName) {
    return textures.at(textureName);
}
