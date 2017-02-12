#ifndef SHADER_HELPER_H
#define SHADER_HELPER_H

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"

/**
 * Data structure that represents a prescence of light
 *
 * X, Y, Z 		- Represents the position of the light
 * R, G, B 		- Represents the color of the light
 * Orientation  - Represents the orientation of the light - Not currently in use
 * Type 		- The type of light represented, currently only sun-esque direction light
 */
typedef struct {
	GLfloat x, y, z;
	GLfloat r, g, b;
	GLfloat orientation;
	GLint type;
} Light;

/**
 * Data structure that represents a mesh material
 *
 * R, G, B 		- Represents the color of the material for each type of lighting (Amb = Ambient, Dif = Diffuse, Spc = Specular)
 * 
 */
typedef struct {
	GLfloat rAmb, gAmb, bAmb;
	GLfloat rDif, gDif, bDif;
	GLfloat rSpc, gSpc, bSpc;
	GLfloat shininess;
} Material;

// TODO(rgarmsen2295): EXTERMINATE ALL GLOBALS

extern Light curLight;

extern std::string RESOURCE_DIR;

// TODO(rgarmsen2295): END GLOBALS

#endif
