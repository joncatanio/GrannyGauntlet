#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include "GLSL.h"

#include "Image.h"

class Cubemap
{
public:

    Cubemap();
    virtual ~Cubemap();
    void loadCubemap();
    void bind();
    void unbind();

private:
    GLuint tid;

    std::vector<std::string> imgNames = {"posx.bmp", "negx.bmp", "posy.bmp", "negy.bmp", "posz.bmp", "negz.bmp" };

};

#endif
