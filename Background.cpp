#define GLEW_STATIC

#include "Background.h"

Background::Background() {
    // Initialize stars and planets
    for (int i = 0; i < 100; ++i) {
        spawnStar();
    }
}

Background::~Background() {
    // Remove VBO deletion
}

void Background::spawnStar() {
    stars.push_back(Star());
}

void Background::update(float deltaTime) {
    for (int i = 0; i < stars.size(); ++i) {
        stars[i].update(deltaTime);
    }
}

void Background::render() {
    glBegin(GL_QUADS);
        // Top-left (black)
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(-1.0f, 1.0f);
        
        // Top-right (black)
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(1.0f, 1.0f);
        
        // Bottom-right (purple)
        glColor3f(0.2f, 0.0f, 0.2f);
        glVertex2f(1.0f, -1.0f);
        
        // Bottom-left (purple)
        glColor3f(0.2f, 0.0f, 0.2f);
        glVertex2f(-1.0f, -1.0f);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render stars and planets
    for (int i = 0; i < stars.size(); ++i) {
        stars[i].render();
    }
    
    glDisable(GL_BLEND);
} 
