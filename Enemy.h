#ifndef ENEMY_H
#define ENEMY_H

#include "Globals.h"
#include "Projectile.h"
#include "ParticleSystem.h"

class Enemy {
  private:
    float x, y;      
    float size;        
    float speedY;      // Vertical speed
    float swayAmplitude; // Amplitude of sine wave
    float swayFrequency; // Frequency of sine wave
    bool active;       
    
    // Vertex and color arrays
    float *vertices;                 // Body vertices
    float *colors;                   // Body colors
    int vertexCount;                 // Number of vertices for the body
    
    GLuint vboVertices; 
    GLuint vboColors;   

    ParticleSystem deathParticles;
    bool isDying;
    float deathTimer;
    static const float DEATH_ANIMATION_TIME = 10.0f;  

    void loadSpriteData(const char* filename); // Load sprite data from file
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

    // New method to set size
    void setSize(float newSize);

    void startDeathAnimation();
};

#endif // ENEMY_H
