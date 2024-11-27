#ifndef PLAYER_H
#define PLAYER_H

#include "Globals.h"

class Player {
  private:
    float x, y;
    float velocityX, velocityY;
    float acceleration;
    float friction;
    float maxSpeed;
    float size;

    // Vertex and color arrays
    float *vertices;
    float *colors;

    // Buffer objects
    GLuint vboVertices, vboColors;

    void initializeBuffers();
    void checkBoundaryCollision();
    void updatePosition();

  public:
    Player(float startX = 0.0f, float startY = 0.0f, float playerSize = 0.1f);
    ~Player();
    void update();
    void render();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    float getX() const;
    float getY() const;
    float getSize() const;
};

#endif // PLAYER_H
