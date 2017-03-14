#include <iostream>
#include "Particle.h"

using namespace std;

float randFloat(float l, float h)
{
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * l + r * h;
}

Particle::Particle(glm::vec3 startPos, float particleMinLiveTime, float particleMaxLiveTime, float particleMaxXYVelocity) :
        mass(1.0f),
        position(0.0f, 0.0f, 0.0f),
        velocity(0.0f, 0.0f, 0.0f),
        lifespan(1.0f),
        tEnd(0.0f),
        scale(1.0f),
        startPosition(startPos),
        particleMinLiveTime_(particleMinLiveTime),
        particleMaxLiveTime_(particleMaxLiveTime),
        particleMaxXYVelocity_(particleMaxXYVelocity)
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
    mass = 1.0f;
    position = glm::vec3(startPosition.x, startPosition.y, startPosition.z);
    velocity = glm::vec3(dieOffPercentage_ * randFloat(-particleMaxXYVelocity_, particleMaxXYVelocity_),
                         dieOffPercentage_ * randFloat(3.0f, 4.0f),
                         dieOffPercentage_ * randFloat(-particleMaxXYVelocity_, particleMaxXYVelocity_));

    lifespan = randFloat(dieOffPercentage_ * particleMinLiveTime_,
                         dieOffPercentage_ * particleMaxLiveTime_);


    tEnd = t + lifespan;
    scale = randFloat(0.2, 1.0f);
}

void Particle::update(float t, float tDiff, float g)
{
    if(t > tEnd) {
        rebirth(t);
    }

    position += tDiff * velocity;

    velocity = glm::vec3(velocity.x, velocity.y + tDiff * mass * g, velocity.z);

}

void Particle::update(float t, float tDiff, float g, float dieOffPercentage)
{
    dieOffPercentage_ = dieOffPercentage;
    if(t > tEnd) {
        rebirth(t);
    }

    position += tDiff * velocity;

    velocity = glm::vec3(velocity.x, velocity.y + tDiff * mass * g, velocity.z);

}

float Particle::getLifePercent(float t)
{
    return 1.0 - ((tEnd-t)/lifespan);
}