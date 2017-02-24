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



private:
    GLuint shadowMapFBO;

    GLuint shadowMap;

    //const GLuint S_WIDTH = 2048, S_HEIGHT = 2048;
    //const GLuint S_WIDTH = 4096, S_HEIGHT = 4096;
    const GLuint S_WIDTH = 16384, S_HEIGHT = 16384;
};
#endif
