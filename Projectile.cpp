#define GLEW_STATIC
#include "Projectile.h"

Projectile::Projectile() : x(0), y(0), velocityY(PROJECTILE_SPEED), active(false) {
    initializeBuffers();
}

Projectile::~Projectile() {
    delete[] vertices;
    delete[] colors;
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboColors);
}

void Projectile::initializeBuffers() {
    // Generate vertex buffer objects
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboColors);

    // Allocate arrays
    vertices = new float[8];  // 4 vertices * 2 coordinates
    colors = new float[12];   // 4 vertices * 3 colors (RGB)

    // Initialize vertex data for a rectangle
    updateVertices();

    // Initialize color data (yellow projectile)
    for (int i = 0; i < 6; i += 3) {
        colors[i] = 0.0f;     // R
        colors[i+1] = 0.0f;   // G
        colors[i+2] = 0.0f;   // B
    }
    
    for (int i = 6; i < 12; i += 3) {
        colors[i] = 1.0f;     // R
        colors[i+1] = 1.0f;   // G
        colors[i+2] = 0.0f;   // B
    }

    // Bind and fill vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    // Bind and fill color buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STATIC_DRAW);
}

void Projectile::updateVertices() {
    vertices[0] = x - PROJECTILE_WIDTH/2; vertices[1] = y - PROJECTILE_HEIGHT/2;  // Bottom left
    vertices[2] = x + PROJECTILE_WIDTH/2; vertices[3] = y - PROJECTILE_HEIGHT/2;  // Bottom right
    vertices[4] = x + PROJECTILE_WIDTH/2; vertices[5] = y + PROJECTILE_HEIGHT/2;  // Top right
    vertices[6] = x - PROJECTILE_WIDTH/2; vertices[7] = y + PROJECTILE_HEIGHT/2;  // Top left
}

void Projectile::spawn(float startX, float startY) {
    x = startX;
    y = startY;
    active = true;
    updateVertices();

    // Update VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), vertices);
}

void Projectile::update() {
    if (!active)
        return;

    y += velocityY;

    // Deactivate if out of bounds
    if (y > SCREEN_TOP) {
        active = false;
        return;
    }

    updateVertices();

    // Update VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), vertices);
}

void Projectile::render() {
    if (!active)
        return;

    glEnableVertexAttribArray(0); // Position

    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(0);
}

bool Projectile::isActive() const { return active; }
