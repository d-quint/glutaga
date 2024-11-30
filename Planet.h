#ifndef PLANET_H
#define PLANET_H

#include "Globals.h"

class Planet {
private:
    float x, y;
    float size;
    float speedY;
    float rotation;
    float rotationSpeed;
    float hue[4];
    
    static const int GRID_SIZE = 11;

public:
    Planet();
    ~Planet();
    void update(float deltaTime);
    void render();
};

#endif // PLANET_H
