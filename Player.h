#ifndef PLAYER_H
#define PLAYER_H

#include "Globals.h"
#include "Projectile.h"
#include "ParticleSystem.h"

class Player {
private:
float x, y;                      // Player position
    float velocityX, velocityY;      // Velocity in X and Y directions
    float acceleration;              // Acceleration of the player
    float friction;                  // Friction applied to the player's movement
    float maxSpeed;                  // Maximum speed of the player
    float playerSize;                      // Size of the player

    // Vertex and color arrays
    float *vertices;                 // Body vertices
    float *colors;                   // Body colors
    int vertexCount;                 // Number of vertices for the body

    // Buffer objects
    GLuint vboVertices, vboHeadVertices, vboColors, vboHeadColors;

    // Rotation-related variables
    bool isRotating;                 // State to indicate if the head is rotating
    float rotationAngle;             // Current rotation angle (in degrees)
    float rotationSpeed;             // Speed of rotation (degrees per frame)
    
    ParticleSystem particleSystem;

    // Current and target colors for the head
    float currentHeadColor[3];
    float targetHeadColor[3];

    void loadSpriteData(const char* filename);
    void initializeBuffers();
    void checkBoundaryCollision();
    void updatePosition();

    bool isDead;
    bool isExploding;
    float deathTimer;
    static const float DEATH_ANIMATION_TIME = 5.0f;

    void (*deathCallback)();

public:
    Player(float startX = 0.0f, float startY = 0.0f, float _playerSize = 0.1f);
    ~Player();

    void update();                   // Update the player (position, rotation, etc.)
    void render();                   // Render the player
    void moveLeft();                 // Move left by adjusting velocity
    void moveRight();                // Move right by adjusting velocity
    void moveUp();                   // Move up by adjusting velocity
    void moveDown();                 // Move down by adjusting velocity

    void startRotation();            // Start rotating the head

    float getX() const;              // Get the current X position
    float getY() const;              // Get the current Y position
    float getSize() const;           // Get the size of the player

    void shoot(std::vector<Projectile*>& projectiles);
    bool checkCollision(float projectileX, float projectileY, float projectileWidth, float projectileHeight);
    
    void startDeathSequence();
    void takeDamage();
    bool isDying() const { return isExploding; }
    
    void setDeathCallback(void (*callback)()) {
        deathCallback = callback;
    }

    bool isPlayerDead() const { return isDead; }

    void reset();
};

#endif // PLAYER_H
