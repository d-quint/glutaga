#define GLEW_STATIC

#include "Player.h"

Player::Player(float startX, float startY, float playerSize)
    : x(startX), y(startY), size(playerSize), velocityX(0), velocityY(0), 
      acceleration(0.001f), friction(0.98f), maxSpeed(0.02f) {
    vertices = NULL;
    colors = NULL;
    initializeBuffers();
}

Player::~Player() {
    delete[] vertices;
    delete[] colors;
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboColors);
}

void Player::initializeBuffers() {
    // Generate vertex buffer objects
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboColors);

    // Allocate arrays
    vertices = new float[8]; // 4 vertices * 2 coordinates
    colors = new float[12];  // 4 vertices * 3 colors (RGB)

    // Initialize vertex data for a square
    vertices[0] = -size;
    vertices[1] = -size; // Bottom left
    vertices[2] = size;
    vertices[3] = -size; // Bottom right
    vertices[4] = size;
    vertices[5] = size; // Top right
    vertices[6] = -size;
    vertices[7] = size; // Top left

    // Initialize color data (white for now)
    for (int i = 0; i < 12; i += 3) {
        colors[i] = 0.0f;     // R
        colors[i + 1] = 1.0f; // G
        colors[i + 2] = 0.0f; // B
    }

    // Bind and fill vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    // Bind and fill color buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Player::checkBoundaryCollision() {
    // Right wall
    if (x + size > SCREEN_RIGHT) {
        x = SCREEN_RIGHT - size;
        velocityX = -velocityX * BOUNCE_FACTOR;
    }
    // Left wall
    if (x - size < SCREEN_LEFT) {
        x = SCREEN_LEFT + size;
        velocityX = -velocityX * BOUNCE_FACTOR;
    }
    // Top wall
    if (y + size > SCREEN_TOP) {
        y = SCREEN_TOP - size;
        velocityY = -velocityY * BOUNCE_FACTOR;
    }
    // Bottom wall
    if (y - size < SCREEN_BOTTOM) {
        y = SCREEN_BOTTOM + size;
        velocityY = -velocityY * BOUNCE_FACTOR;
    }
}

void Player::update() {
    // Apply friction
    velocityX *= friction;
    velocityY *= friction;

    // Update position based on velocity
    x += velocityX;
    y += velocityY;

    // Check for wall collisions
    checkBoundaryCollision();

    // Update vertex positions
    updatePosition();

    // Stop completely if moving very slowly
    if (std::abs(velocityX) < 0.0001f)
        velocityX = 0;
    if (std::abs(velocityY) < 0.0001f)
        velocityY = 0;
}

void Player::updatePosition() {
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    if (ptr) {
        ptr[0] = x - size; ptr[1] = y - size;  // Bottom left
        ptr[2] = x + size; ptr[3] = y - size;  // Bottom right
        ptr[4] = x + size; ptr[5] = y + size;  // Top right
        ptr[6] = x - size; ptr[7] = y + size;  // Top left
        
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Player::render() {
    glEnableVertexAttribArray(0); // Position

    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glDrawArrays(GL_QUADS, 0, 4);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
}

void Player::moveLeft() {
    velocityX -= acceleration;
    if (velocityX < -maxSpeed) velocityX = -maxSpeed;
}

void Player::moveRight() {
    velocityX += acceleration;
    if (velocityX > maxSpeed) velocityX = maxSpeed;
}

void Player::moveUp() {
    velocityY += acceleration;
    if (velocityY > maxSpeed) velocityY = maxSpeed;
}

void Player::moveDown() {
    velocityY -= acceleration;
    if (velocityY < -maxSpeed) velocityY = -maxSpeed;
}

// Getters
float Player::getX() const { return x; }
float Player::getY() const { return y; }
float Player::getSize() const { return size; }
