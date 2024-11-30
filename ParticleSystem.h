#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Particle.h"
#include "Globals.h"

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
    void emit(float x, float y, float vx, float vy, float life, float size, bool isDamage = false);
    void emitExplosion(float x, float y);  // Explosion effect
};

#endif 
