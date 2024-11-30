#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float _emissionRate)
    : emissionRate(_emissionRate), timeSinceLastEmission(0) {
    srand(time(0));
}

void ParticleSystem::update(float deltaTime) {
    // Update existing particles
    for (int i = particles.size() - 1; i >= 0; i--) {
        if (!particles[i].update(deltaTime)) {
            particles.erase(particles.begin() + i);
        }
    }
}

void ParticleSystem::render() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i].render();
    }
    
    glDisable(GL_BLEND);
}

void ParticleSystem::emit(float x, float y) {
    // Create new particles at the ship's position
    for (int i = 0; i < 5; i++) {
        // Randomize initial position slightly
        float startX = x + ((rand() % 100 - 50) / 1000.0f);
        float startY = y + ((rand() % 100 - 50) / 1000.0f);
        
        // Much faster downward velocity (always negative to go down)
        float baseVY = -0.5f - (rand() % 100) / 100.0f;  
        
        // Faster horizontal spread
        float baseVX = ((rand() % 100 - 50) / 400.0f);     
        
        // Shorter life since they're moving faster
        float life = 0.5f + (rand() % 100) / 200.0f;  // 0.5 to 1.0 seconds
        
        // Smaller initial size that will grow
        float size = 1.0f + (rand() % 20) / 10.0f;    // 1.0 to 3.0 initial size
        
        particles.push_back(Particle(startX, startY, baseVX, baseVY, life, size));
    }
}

void ParticleSystem::emit(float x, float y, float vx, float vy, float life, float size, bool isDamage) {
    particles.push_back(Particle(x, y, vx, vy, life, size, isDamage));
}

void ParticleSystem::emitExplosion(float x, float y) {
    std::cout << "Emitting explosion particles..." << std::endl;
    
    // Create explosion particles in all directions
    for (int i = 0; i < 50; i++) {
        float startX = x + ((rand() % 100 - 50) / 1000.0f);
        float startY = y + ((rand() % 100 - 50) / 1000.0f);
        
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 1.0f + (rand() % 100) / 100.0f;
        
        float baseVX = speed * cos(angle);
        float baseVY = speed * sin(angle);
        
        float life = 5.0f + (rand() % 100) / 20.0f;  // 5.0 to 10.0 seconds
        float size = 2.0f + (rand() % 30) / 10.0f;   // 2.0 to 5.0 size
        
        particles.push_back(Particle(startX, startY, baseVX, baseVY, life, size, true));
    }
    
    std::cout << "Created " << particles.size() << " explosion particles" << std::endl;
}
