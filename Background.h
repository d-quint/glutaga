#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Globals.h"

#include "Star.h"
#include "Planet.h"

class Background {
private:
    std::vector<Star> stars;
    std::vector<Planet> planets;

    void spawnStar();
    void spawnPlanet();

public:
    Background();
    ~Background();
    void update(float deltaTime);
    void render();
};

#endif // BACKGROUND_H 
