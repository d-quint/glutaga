#ifndef PARTICLE_H
#define PARTICLE_H

#include "Globals.h"

#define GRAVITY 0.5f
#define RANDOM_DRIFT 0.00002f

class Particle {
public:
    float x, y;           // Position
    float vx, vy;         // Velocity
    float life;           // Current life (decreases over time)
    float maxLife;        // Maximum life span
    float size;          // Size of the particle
    float r, g, b, a;     // Color and alpha (opacity)

    Particle(float x, float y, float vx, float vy, float life, float size, bool isExplosion = false);
    bool update(float deltaTime);  // Returns false when particle dies
    void render();
};

#endif 
