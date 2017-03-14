#include <iostream>
#include "Particle.h"

using namespace std;

float randFloat(float l, float h)
{
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * l + r * h;
}

Particle::Particle(glm::vec3 startPos) :
        m(1.0f),
        x(0.0f, 0.0f, 0.0f),
        v(0.0f),
        direction(0.0f, 0.0f, 0.0f),
        lifespan(1.0f),
        tEnd(0.0f),
        scale(1.0f),
        startPosition(startPos)
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
    x = glm::vec3(startPosition.x, startPosition.y, startPosition.z);
    //x.z = randFloat(-3.0, -2.0);

    v = randFloat(3.0f, 4.0f);
    direction = glm::vec3(randFloat(-0.3, 0.3), 1.0f, randFloat(-0.3, 0.3));
    direction = glm::normalize(direction);

    lifespan = randFloat(1.0f, 1.5f);


    tEnd = t + lifespan;
    scale = randFloat(0.2, 1.0f);
}

void Particle::update(float t, float tDiff, float g)
{
    if(t > tEnd) {
        rebirth(t);
    }

    //very simple update
    x += tDiff * v * direction;

    v += tDiff * m * g;

}

float Particle::getLifePercent(float t)
{
    return 1.0 - ((tEnd-t)/lifespan);
}