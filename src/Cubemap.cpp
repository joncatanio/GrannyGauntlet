#include "Cubemap.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include "GLSL.h"
#include <iostream>

Cubemap::Cubemap(std::string path, std::string fileExtension) :
    path_(path),
    fileExtension_(fileExtension) {}

Cubemap::~Cubemap() {}

void Cubemap::loadCubemap() {

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tid_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tid_);

    for (unsigned int i = 0; i < imgNames.size(); i++) {

        //TODO(nurgan) move to ResourceManager and handle loading from there
        std::string imgPath = "../resources/" + path_ + imgNames[i] + "." + fileExtension_;

       Image* image = new Image(imgPath);

        if(image->components == 3) {
            // RGB texture

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, image->width,  image->height,
                         0, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*) image->getImageData());
        } else if (image->components == 4) {
            // RBGA texture
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGBA, image->width,  image->height,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, (GLubyte*) image->getImageData());
        }

    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tid_);
}

void Cubemap::unbind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}