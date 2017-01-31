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

#define NUM_MATERIALS	  5

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
extern float aspect;

// Global height and width variables representing the view resolution
extern int g_width, g_height;

// Previous x & y pos for the mouse
extern float prevX;
extern float prevY;

// Materials
extern std::shared_ptr<Material> obsidian;
extern std::shared_ptr<Material> green;
extern std::shared_ptr<Material> jade;
extern std::shared_ptr<Material> pearl;
extern std::shared_ptr<Material> brass;

extern Light curLight;

extern std::string RESOURCE_DIR;

// Main application window
extern GLFWwindow *window;

// TODO(rgarmsen2295): END GLOBALS

#endif
