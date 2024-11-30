#define GLEW_STATIC

#include "Planet.h"

// Planet pixel art pattern (11x11)
const bool planetPattern[11][11] = {
    {0,0,0,0,1,1,1,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,1,1,1,0,0,0,0}
};

Planet::Planet() {
    x = static_cast<float>(rand()) / RAND_MAX * 
        (SCREEN_RIGHT_GAMEPLAY - SCREEN_LEFT_GAMEPLAY) + SCREEN_LEFT_GAMEPLAY;
    y = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
    
    size = 0.25f + (static_cast<float>(rand()) / RAND_MAX * 0.25f);
    speedY = -0.05f - (static_cast<float>(rand()) / RAND_MAX * 0.25f);
    rotation = 0.0f;
    rotationSpeed = -2.0f + (static_cast<float>(rand()) / RAND_MAX * 4.0f);
    
    // Random dark color
    hue[0] = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.2f;
    hue[1] = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.1f;
    hue[2] = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.2f;
}

Planet::~Planet() {
}

void Planet::update(float deltaTime) {
    y += speedY * deltaTime;
    rotation += rotationSpeed * deltaTime;
    
    if (y < -1.5f) {
        x = static_cast<float>(rand()) / RAND_MAX * 
            (SCREEN_RIGHT_GAMEPLAY - SCREEN_LEFT_GAMEPLAY) + SCREEN_LEFT_GAMEPLAY;
        y = 1.5f + (static_cast<float>(rand()) / RAND_MAX * 0.5f);
        
        size = 0.25f + (static_cast<float>(rand()) / RAND_MAX * 0.25f);
		speedY = -0.05f - (static_cast<float>(rand()) / RAND_MAX * 0.25f);
		rotationSpeed = -2.0f + (static_cast<float>(rand()) / RAND_MAX * 4.0f);
		
	    hue[0] = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.2f;
	    hue[1] = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.1f;
	    hue[2] = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.2f;
    }
}

void Planet::render() {
    float pixelSize = size / GRID_SIZE;
    
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    
    glBegin(GL_QUADS);
    for(int i = 0; i < GRID_SIZE; i++) {
        for(int j = 0; j < GRID_SIZE; j++) {
            if (planetPattern[i][j]) {
                // Calculate pixel position relative to planet center
                float px = (j - GRID_SIZE/2.0f) * pixelSize;
                float py = (GRID_SIZE/2.0f - i) * pixelSize;
                
                glColor3f(hue[0], hue[1], hue[2]);
                glVertex2f(px, py);
                glVertex2f(px + pixelSize, py);
                glVertex2f(px + pixelSize, py - pixelSize);
                glVertex2f(px, py - pixelSize);
            }
        }
    }
    glEnd();
    
    glPopMatrix();
} 
