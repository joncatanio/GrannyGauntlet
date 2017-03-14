#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "MatrixStack.h"

class Particle
{
public:
    Particle();
    virtual ~Particle();
    void load();
    void rebirth(float t);
    void update(float t, float h, float g);
    const glm::vec3 &getPosition() const { return x; };
    const glm::vec3 &getDirection() const { return direction; };
    const float &getVelocity() const { return v; };

private:
    float m; // mass
    glm::vec3 x; // position
    float v; // velocity
    glm::vec3 direction;
    float lifespan; // how long this particle lives
    float tEnd;     // time this particle dies
    float scale;
};

#endif