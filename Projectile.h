#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Globals.h"

class Projectile {
  private:
    float x, y, velocityY;
    bool active;
    bool fromEnemy;

    // Vertex and color arrays
    float *vertices;
    float *colors;

    // Buffer objects
    GLuint vboVertices, vboColors;

    void initializeBuffers();
    void updateVertices();

  public:
    Projectile(bool fromEnemy);
    ~Projectile();
    void spawn(float startX, float startY);
    void update();
    void render();
    void deactivate();
    bool isActive() const;
    
    float getX() const;
    float getY() const;
};

#endif // PROJECTILE_H
