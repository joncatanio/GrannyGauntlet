#ifndef TEXTURE_H
#define TEXTURE_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <memory>

#include "Program.h"
#include "Image.h"


class Texture
{
public:
    Texture();
    virtual ~Texture();

    std::string name;

    // loads the texture under the specified path. channels = channels per pixel, 3 for RGB, 4 for RGBA
    void loadTexture(std::string path, std::string newName);
    void bind(int newUnit, const std::shared_ptr<Program> prog);
    void unbind();
    void setHandle(GLint h);
    GLint getHandle();


private:

	GLuint tid;
	Image* image;
    int unit;
    GLint handle;

};


#endif
