#include "ParticleSystem.h"

#include <algorithm>

#include "ShaderManager.h"

//
#include "ShapeManager.h"
#include "MaterialManager.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::init(float numberOfParticles, glm::vec3 position) {


    //numberOfParticles_ = numberOfParticles;
    position_ = position;

    for(int i = 0; i < numberOfParticles_; i++) {
        std::shared_ptr<Particle> particle = std::make_shared<Particle>(position_);
        particles.push_back(particle);
        particle->load();
    }

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &pointsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointsbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &lifebuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lifebuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lifes), NULL, GL_STREAM_DRAW);

    //texture = new Texture();
    //texture.loadTexture("../resources/water_atlas.jpg", "water_atlas");
    texture.loadTexture("../resources/water_atlas.jpg", "water_atlas");
}

class ParticleSorter {
public:
    bool operator()(const std::shared_ptr<Particle> p0, const std::shared_ptr<Particle> p1) const
    {
        // Particle positions in world space
        const glm::vec3 &x0 = p0->getPosition();
        const glm::vec3 &x1 = p1->getPosition();
        // Particle positions in camera space
        glm::vec4 x0w = C * glm::vec4(x0.x, x0.y, x0.z, 1.0f);
        glm::vec4 x1w = C * glm::vec4(x1.x, x1.y, x1.z, 1.0f);
        return x0w.z < x1w.z;
    }

    glm::mat4 C; // current camera matrix
};
ParticleSorter sorter;

void ParticleSystem::update(float t, float tDiff, std::shared_ptr<MatrixStack> V) {
    //std::cout << "update" << std::endl;
    //update the particles
    for(auto &particle : particles) {
        particle->update(t, tDiff, gravity);

        //std::cout << particle->getPosition().x << " " << particle->getPosition().y << " " << particle->getPosition().z << std::endl;
    }
    //t += tDiff;

    // Sort the particles by Z
    auto temp = std::make_shared<MatrixStack>();
    //temp->rotate(camRot, vec3(0, 1, 0));
    sorter.C = V->topMatrix();
    sort(particles.begin(), particles.end(), sorter);


    //

    glm::vec3 pos;

    //go through all the particles and update the CPU buffer
    for (int i = 0; i < numberOfParticles_; i++) {
        pos = particles[i]->getPosition();
        points[i*3+0] = pos.x;
        points[i*3+1] = pos.y;
        points[i*3+2] = pos.z;
        lifes[i] = particles[i]->getLifePercent(t);
        //std::cout << particles[i]->getLifePercent(t) << std::endl;
    }

    //update the GPU data
    glBindBuffer(GL_ARRAY_BUFFER, pointsbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*numberOfParticles_*3, points);
    glBindBuffer(GL_ARRAY_BUFFER, lifebuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lifes), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*numberOfParticles_, lifes);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ParticleSystem::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(50.0f);

    //prog->bind();
    ShaderManager& shaderManager = ShaderManager::instance();
    const std::shared_ptr<Program> shaderProgram = shaderManager.bindShader("Particle");

    texture.bind(0, shaderProgram);


    glUniformMatrix4fv(shaderProgram->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
    glUniformMatrix4fv(shaderProgram->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
    glUniformMatrix4fv(shaderProgram->getUniform("M"), 1, GL_FALSE, glm::value_ptr(M->topMatrix()));
    glm::mat4 tiM = glm::transpose(glm::inverse(M->topMatrix()));
    glUniformMatrix4fv(shaderProgram->getUniform("tiM"), 1, GL_FALSE, glm::value_ptr(tiM));

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, pointsbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,(void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, lifebuffer);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0,(void*)0);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    // Draw the points !
    glDrawArraysInstanced(GL_POINTS, 0, 1, numberOfParticles_);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDisable(GL_BLEND);
    texture.unbind();

    //prog->unbind();
    shaderManager.unbindShader();

}