#ifndef ENEMY_H
#define ENEMY_H

#include "Globals.h"
#include "Projectile.h"

class Enemy {
  private:
    float x, y;      
    float size;        
    float speedY;      // Vertical speed
    float swayAmplitude; // Amplitude of sine wave
    float swayFrequency; // Frequency of sine wave
    bool active;       
    
    GLuint vboVertices; 
    GLuint vboColors;   

    void initializeBuffers();
    void updateVertices();

  public:
    Enemy();
    ~Enemy();

    void spawn();   
    void shoot(std::vector<Projectile*>& projectileList);
    void update(float deltaTime);
    void render();     
    void deactivate(); 
    bool isActive() const;
    bool checkCollision(float projectileX, float projectileY, float projectileWidth, float projectileHeight);
};

#endif // ENEMY_H

