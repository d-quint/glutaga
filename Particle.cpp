#include "Particle.h"
#include <cstdlib>

Particle::Particle(float _x, float _y, float _vx, float _vy, float _life, float _size, bool isDamage)
    : x(_x), y(_y), vx(_vx), vy(_vy), life(_life), maxLife(_life), size(_size) {
    
    if (isDamage) {
        // Red and orange colors for damage particles
        switch(rand() % 3) {
            case 0: // Bright green
                r = 0.2f;
                g = 0.6f;
                b = 0.2f;
                break;
            case 1: // Orange
                r = 1.0f;
                g = 0.6f;
                b = 0.0f;
                break;
            case 2: // Yellow
                r = 1.0f;
                g = 0.9f;
                b = 0.0f;
                break;
        }
        a = 0.8f;  
    } else {
        // Original particle colors (rest of the existing color logic)
        switch(rand() % 4) {
            case 0: // Grey (smoke)
                r = 0.7f;
                g = 0.7f;
                b = 0.7f;
                break;
            case 1: // Orange
                r = 1.0f;
                g = 0.5f;
                b = 0.0f;
                break;
            case 2: // Red
                r = 1.0f;
                g = 0.2f;
                b = 0.0f;
                break;
            case 3: // White
                r = 1.0f;
                g = 1.0f;
                b = 1.0f;
                break;
        }
        a = 0.8f;
    }
}

bool Particle::update(float deltaTime) {
    // Apply gravity (pulls particles down)
    vy -= GRAVITY * deltaTime;
    
    // Add random horizontal drift (smoke wavering)
    vx += (((rand() % 100) / 100.0f) - 0.5f) * RANDOM_DRIFT;
    
    // Updating position
    x += vx * deltaTime;
    y += vy * deltaTime;
    
    // Updating life and alpha
    life -= deltaTime;
    a = (life / maxLife) * 0.7f;  // Fade out gradually
    
    // Gradually increase size as particle rises (smoke spreading)
    size += deltaTime * 0.5f;
    
    return life > 0;
}

void Particle::render() {
    glPointSize(size);
    glBegin(GL_POINTS);
    glColor4f(r, g, b, a);
    glVertex2f(x, y);
    glEnd();
} 
