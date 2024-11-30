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
    
    // Score tracking
    int currentScore;
    float scoreAnimTime;  // Tracks time for score animation
    static const float SCORE_BOB_SPEED = 2.0f;  // Speed of up/down motion
    static const float SCORE_BOB_AMOUNT = 0.01f; // Amount of up/down motion
    
    int highScore;
    
    void loadHighScore();

  public:
    UI(Player* _player, float maxHealth = 100.0f) 
        : player(_player),
          maxPlayerHealth(maxHealth), 
          playerHealth(maxHealth), 
          displayedHealth(maxHealth),
          animationSpeed(2.0f),
          x(0.0f),
          y(0.0f),
          currentScore(0),
          scoreAnimTime(0.0f),
          highScore(0) {
        loadHighScore();
    }
    
    void render();
    void update(float deltaTime);
    void setHealth(float health);
    void setPosition(float newX, float newY);
    float getHealth() const { return playerHealth; }
    void damage(float amount);
    void heal(float amount);
    void addScore(int amount);
    int getScore() const { return currentScore; }
    void saveHighScore();
};

#endif // UI_H 
