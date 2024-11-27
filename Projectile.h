#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Globals.h"

class Projectile {
  private:
    float x, y, velocityY;
    bool active;

    // Vertex and color arrays
    float *vertices;
    float *colors;

    // Buffer objects
    GLuint vboVertices, vboColors;

    void initializeBuffers();
    void updateVertices();

  public:
    Projectile();
    ~Projectile();
    void spawn(float startX, float startY);
    void update();
    void render();
    bool isActive() const;
};

#endif // PROJECTILE_H
