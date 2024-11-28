#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>
#include "Particle.h"

class ParticleSystem {
private:
    std::vector<Particle> particles;
    float emissionRate;    // Particles per second
    float timeSinceLastEmission;

public:
    ParticleSystem(float emissionRate = 60.0f);
    void update(float deltaTime);
    void render();
    void emit(float x, float y);  // Regular emission
    void emitExplosion(float x, float y);  // Explosion effect
};

#endif 