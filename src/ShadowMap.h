#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include "GLSL.h"

#define GLEW_STATIC
#include <GL/glew.h>

class ShadowMap {

public:

    ShadowMap();

    ~ShadowMap();

    void bindForShadowPass();

    void bindForDraw();

    GLuint getShadowMap();

    //static constexpr GLuint SM_WIDTH = 4096, SM_WIDTH = 4096;
    static constexpr GLuint SM_WIDTH = 8192, SM_HEIGHT = 8192;
    //static constexpr GLuint SM_WIDTH = 16384, SM_WIDTH = 16384;

private:
    GLuint shadowMapFBO;

    GLuint shadowMap;

};
#endif
