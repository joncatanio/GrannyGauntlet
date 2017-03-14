#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"

#include <memory>
#include <string>
#include <vector>

#include "Texture.h"

class ParticleSystem
{
public:
    ParticleSystem();
    virtual ~ParticleSystem();

    void init(float numberOfParticles, glm::vec3 position);
    void update(float t, float tDiff, std::shared_ptr<MatrixStack> V);

    void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

private:
    glm::vec3 startPosition;
    Texture texture;

    std::vector<std::shared_ptr<Particle>> particles;

    static const int numberOfParticles_ = 200;
    glm::vec3 position_;

    float gravity = -1.0;

    GLfloat points[numberOfParticles_ * 3];
    GLfloat lifes[numberOfParticles_];
    GLuint pointsbuffer;
    GLuint lifebuffer;
    GLuint VertexArrayID;

};



#endif