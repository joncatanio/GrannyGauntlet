#include "Texture.h"
#include "GLSL.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture() {}

Texture::~Texture() {}

void Texture::loadTexture(std::string path, std::string newName) {

    name = newName;

    unsigned char* textureData = stbi_load( path.c_str() , &width, &height, &components, STBI_rgb_alpha );

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);

    if(components == 3) {
        // RGB texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*) textureData);
    } else if (components == 4) {
        // RBGA texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLubyte*) textureData);
    }

    // generate MipMap and set minification ang magnification filter
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    // texture data not needed anymore
    stbi_image_free(textureData);

}

void Texture::bind(int newUnit)
{
    unit = newUnit;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tid);
<<<<<<< HEAD
    //glUniform1i(0, unit);
=======
<<<<<<< HEAD
    //glUniform1i(0, unit);
=======
    glUniform1i(0, unit);
>>>>>>> b8d915642ae53926ad0ab23766b04c966b3b4222
>>>>>>> 144ca15d42d66cc466fa1fff2b2d088a1a5124ae
}

void Texture::unbind() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setHandle(GLint h) {
    handle = h;
}

GLint Texture::getHandle() {
    return handle;
}