#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"

#include <memory>
#include <string>
#include <vector>

#include "Texture.h"

class ParticleSystem : public std::enable_shared_from_this<ParticleSystem>
{
public:
    ParticleSystem();
    virtual ~ParticleSystem();

    void init(float numberOfParticles, glm::vec3 position);
    void update(float t, float tDiff, std::shared_ptr<MatrixStack> V);
    void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);
    float getMaxRadius();
    glm::vec3 getPosition();

private:
    glm::vec3 startPosition;
    Texture texture;

    float gravity = -2.0;
    float lifespan = 15.0;
    float dieOffSpan = 5.0;
    float maxParticleSize = 70.0;
    float minParticleSize = 10.0;
    float maxScaleDist = 35.0;

    float particleMinLiveTime = 0.5;
    float particleMaxLiveTime = 2.5;
    float particleMaxXYVelocity = 1.0;

    float lifeTime = 0.0;
    std::vector<std::shared_ptr<Particle>> particles;
    glm::vec3 position_;
    int numberOfParticles_;

    GLuint pointsbuffer;
    GLuint lifebuffer;
    GLuint VertexArrayID;


    void scaleParticles();

};



#endif