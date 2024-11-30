#ifndef UI_H
#define UI_H

#include "Globals.h"
#include "Player.h"

class UI {
private:
    float playerHealth;
    float maxPlayerHealth;
    float displayedHealth;  // For smooth animation
    float animationSpeed;   // Controls how fast the health bar updates
    float x, y;            // Position of the UI
    Player* player;

public:
    UI(Player* _player, float maxHealth = 100.0f) 
        : player(_player),
          maxPlayerHealth(maxHealth), 
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
    bool isHealthAnimationComplete() const;
};

#endif // UI_H 
