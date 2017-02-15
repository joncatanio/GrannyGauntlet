#include "Cubemap.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include "GLSL.h"

Cubemap::Cubemap() {}

Cubemap::~Cubemap() {}

void Cubemap::loadCubemap() {

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tid);

    for (unsigned int i = 0; i < imgNames.size(); i++) {

        //TODO(nurgan) move to ResourceManager and handle loading from there
        std::string imgPath = "../resources/skybox/" + imgNames[i];

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

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
}

void Cubemap::unbind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}