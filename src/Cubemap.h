#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include "GLSL.h"

#include "Image.h"

class Cubemap
{
public:

    Cubemap(std::string path, std::string fileExtension);
    virtual ~Cubemap();
    void loadCubemap();
    void bind();
    void unbind();

private:
    GLuint tid_;
    std::string path_;
    std::string fileExtension_;

    //filenames to be iterated
    std::vector<std::string> imgNames = {"posx", "negx", "posy", "negy", "posz", "negz" };
};

#endif
