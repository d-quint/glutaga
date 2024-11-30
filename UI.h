#ifndef UI_H
#define UI_H

#include "Globals.h"

class UI {
private:
    float playerHealth;
    float maxPlayerHealth;
    float displayedHealth;  // For smooth animation
    float animationSpeed;   // Controls how fast the health bar updates
    float x, y;            // Position of the UI

public:
    UI(float maxHealth = 100.0f) 
        : maxPlayerHealth(maxHealth), 
          playerHealth(maxHealth), 
          displayedHealth(maxHealth),
          animationSpeed(2.0f),
          x(0.0f),
          y(0.0f) {}
    
    void render();
    void update(float deltaTime);
    void setHealth(float health);
    void setPosition(float newX, float newY);
    float getHealth() const { return playerHealth; }
    void damage(float amount);
    void heal(float amount);
};

#endif // UI_H 
