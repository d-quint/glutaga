#ifndef STAR_H
#define STAR_H

#include "Globals.h"

class Star {
private:
    float x, y;
    float size;
    float speedY;
    float baseAlpha;     // Base opacity that determines speed
    float currentAlpha;  // Current opacity for twinkle effect
    GLuint vboVertices, vboColors;

public:
    Star();
    ~Star();
    void update(float deltaTime);
    void render();
};

#endif // STAR_H 
