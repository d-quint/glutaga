#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Globals.h"

#include "Star.h"
// #include "Planet.h"

class Background {
private:
    std::vector<Star> stars;
    // std::vector<Planet> planets;
    GLuint vboGradientVertices;
    GLuint vboGradientColors;

    void spawnStar();
    // void spawnPlanet();
    void initGradient();

public:
    Background();
    ~Background();
    void update(float deltaTime);
    void render();
};

#endif // BACKGROUND_H 
