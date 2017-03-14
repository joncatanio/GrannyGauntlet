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
    Particle(glm::vec3 startPos, float particleMinLiveTime, float particleMaxLiveTime, float particleMaxXYVelocity);
    virtual ~Particle();
    void load();
    void rebirth(float t);
    void update(float t, float tDiff, float g);
    void update(float t, float tDiff, float g, float dieOffPercentage);
    const glm::vec3 &getPosition() const { return position; };
    const glm::vec3 &getVelocity() const { return velocity; };
    float getLifePercent(float t);

private:
    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
    float lifespan;
    float tEnd;
    float scale;
    glm::vec3 startPosition;
    float dieOffPercentage_ = 1.0;

    float particleMinLiveTime_;
    float particleMaxLiveTime_;
    float particleMaxXYVelocity_;
};

#endif