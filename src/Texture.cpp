#include "Texture.h"
#include "GLSL.h"

Texture::Texture() {}

Texture::~Texture() {}

void Texture::loadTexture(std::string path, std::string newName) {

    name = newName;

    image = new Image(path);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);

    if(image->components == 3) {
        // RGB texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*) image->getImageData());
    } else if (image->components == 4) {
        // RBGA texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLubyte*) image->getImageData());
    }

    // generate MipMap and set minification ang magnification filter
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::bind(int newUnit)
{
    unit = newUnit;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tid);
    //glUniform1i(0, unit);
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