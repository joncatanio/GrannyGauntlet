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


    numberOfParticles_ = numberOfParticles;
    position_ = position;

    for(int i = 0; i < numberOfParticles_; i++) {
        std::shared_ptr<Particle> particle = std::make_shared<Particle>(position_, particleMinLiveTime, particleMaxLiveTime, particleMaxXYVelocity);
        particles.push_back(particle);
        particle->load();
    }

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &pointsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointsbuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfParticles_ * 3, NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &lifebuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lifebuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfParticles_, NULL, GL_STREAM_DRAW);

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

void ParticleSystem::update(float totalTime, float deltaTime, std::shared_ptr<MatrixStack> V) {
    lifeTime += deltaTime;
    if(lifeTime > lifespan) {
        GameManager::instance().getGameWorld().rmParticleSystem(shared_from_this());
    }
    for(auto &particle : particles) {
        if (lifeTime >= lifespan - dieOffSpan) {
            float dieOff = (lifespan - lifeTime) / dieOffSpan;
            dieOff = std::max(0.3f, dieOff);
            particle->update(totalTime, deltaTime, gravity, dieOff);
        } else {
            particle->update(totalTime, deltaTime, gravity);
        }
    }
    // sort by z, dependent on player camera
    auto temp = std::make_shared<MatrixStack>();
    sorter.C = V->topMatrix();
    sort(particles.begin(), particles.end(), sorter);


	size_t sizeOfPoints = numberOfParticles_ * 3 * sizeof(GLfloat);
	GLfloat *points = (GLfloat *) malloc(sizeOfPoints);

	size_t sizeOfLifes = numberOfParticles_ * sizeof(GLfloat);
	GLfloat *lifes = (GLfloat *) malloc(sizeOfLifes);

    glm::vec3 pos;
    for (int i = 0; i < numberOfParticles_; i++) {
        pos = particles[i]->getPosition();
        points[i*3+0] = pos.x;
        points[i*3+1] = pos.y;
        points[i*3+2] = pos.z;
        lifes[i] = particles[i]->getLifePercent(totalTime);
    }

    glBindBuffer(GL_ARRAY_BUFFER, pointsbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeOfPoints, NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*numberOfParticles_*3, points);
    glBindBuffer(GL_ARRAY_BUFFER, lifebuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeOfLifes, NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*numberOfParticles_, lifes);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	free(points);
	free(lifes);
}

void ParticleSystem::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    scaleParticles();

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
    glDrawArraysInstanced(GL_POINTS, 0, 1, numberOfParticles_);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDisable(GL_BLEND);
    texture.unbind();

    shaderManager.unbindShader();

}

void ParticleSystem::scaleParticles() {

    glm::vec3 eye = GameManager::instance().getCamera().getEye();
    float distToPs = distance(glm::vec2(position_.x, position_.z), glm::vec2(eye.x, eye.z));

    float pSize;
    float sUnit = (maxParticleSize - minParticleSize) / maxScaleDist;

    if(distToPs >= maxScaleDist){
        pSize = minParticleSize;
    } else {
        float distScale = distToPs - maxScaleDist;
        distScale *= -1;
        pSize = minParticleSize + distScale * sUnit;
    }

    glPointSize(pSize);
}

float ParticleSystem::getMaxRadius() {
    return particleMaxXYVelocity * particleMaxLiveTime;
}

glm::vec3 ParticleSystem::getPosition() {
    return position_;
}