#define GLEW_STATIC

#include "Enemy.h"
#include "Projectile.h"

#include <cmath> // For sine wave calculations
#include <cstdlib> // For random spawn

Enemy::Enemy() : x(0), y(0), size(0.1f), 
				 speedY(0.0001f + static_cast<float>(rand()) / RAND_MAX * 0.0003f), 
				 swayAmplitude(0.001f + static_cast<float>(rand()) / RAND_MAX * 0.003f), 
				 swayFrequency(4.0f), active(false) {
    initializeBuffers();
}

Enemy::~Enemy() {
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboColors);
}

void Enemy::initializeBuffers() {
    // Generate VBOs
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboColors);

    // Allocate vertex and color arrays
    float vertices[8];  // 4 vertices * 2 coordinates
    float colors[12];   // 4 vertices * 3 colors (RGB)

    // Initialize vertex data (square)
    vertices[0] = -size; vertices[1] = -size; // Bottom left
    vertices[2] = size;  vertices[3] = -size; // Bottom right
    vertices[4] = size;  vertices[5] = size;  // Top right
    vertices[6] = -size; vertices[7] = size;  // Top left

    // Initialize color data (red square)
    for (int i = 0; i < 12; i += 3) {
        colors[i] = 1.0f;     // R
        colors[i + 1] = 0.0f; // G
        colors[i + 2] = 0.0f; // B
    }

    // Upload vertex data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // Upload color data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Enemy::spawn() {
    x = static_cast<float>(rand()) / RAND_MAX * (SCREEN_RIGHT - SCREEN_LEFT) + SCREEN_LEFT;
    y = SCREEN_TOP + size; // Start just above the top of the screen
    active = true;
}

void Enemy::shoot(std::vector<Projectile*>& projectileList) {
    if (!active) return;

    for (int i = 0; i < projectileList.size(); i++) {
        if (!projectileList[i]->isActive()) {
            projectileList[i]->spawn(x, y - size);
            break;
        }
    }
}


void Enemy::update(float deltaTime) {
    if (!active) return;

    // Update vertical position
    y -= speedY * deltaTime;

    // Update horizontal position with sine wave motion
    x += swayAmplitude * std::sin(swayFrequency * y);

    // Deactivate if it goes below the screen
    if (y < SCREEN_BOTTOM - size) {
        active = false;
    }
}

void Enemy::render() {
    if (!active)
        return;
        
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    glEnableVertexAttribArray(0); // Position

    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glDrawArrays(GL_QUADS, 0, 4);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    
    glPopMatrix();
}

void Enemy::deactivate() {
    active = false;
}

bool Enemy::isActive() const {
    return active;
}

bool Enemy::checkCollision(float projectileX, float projectileY, float projectileWidth, float projectileHeight) {
    if (!active) return false;

    // Axis-aligned bounding box (AABB) collision detection
    bool collisionX = x + size > projectileX - projectileWidth / 2 && x - size < projectileX + projectileWidth / 2;
    bool collisionY = y + size > projectileY - projectileHeight / 2 && y - size < projectileY + projectileHeight / 2;

    if (collisionX && collisionY) {
        deactivate(); // Deactivate if hit
        return true;
    }

    return false;
}
