#include <iostream>
#include "Particle.h"

using namespace std;

float randFloat(float l, float h)
{
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * l + r * h;
}

Particle::Particle() :
        m(1.0f),
        x(0.0f, 0.0f, 0.0f),
        v(0.0f),
        direction(0.0f, 0.0f, 0.0f),
        lifespan(1.0f),
        tEnd(0.0f),
        scale(1.0f)
{
}

Particle::~Particle()
{
}

void Particle::load()
{
    // Random initialization
    rebirth(0.0f);
}

/* all particles born at the origin */
void Particle::rebirth(float t)
{
    m = 1.0f;
    x.x = -0.4;
    x.y = 0.35;
    x.z = randFloat(-3.0, -2.0);

    v = randFloat(-0.3f, 0.3f);
    direction = glm::vec3(0.0f, 1.0f, 0.0f);

    lifespan = randFloat(1.0f, 5.0f);


    tEnd = t + lifespan;
    scale = randFloat(0.2, 1.0f);
}

void Particle::update(float t, float h, float g)
{
    if(t > tEnd) {
        rebirth(t);
    }

    //very simple update
    x += h * v;

    v += m*g;

}