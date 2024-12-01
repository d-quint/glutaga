#include "UI.h"
#include <iostream>

const char* HIGH_SCORE_FILE = "highscore.dat";

void UI::render() {
    // Render game over screen on top of everything
    if (isGameOver) {
        gameOver->render();
    }
    
    // Always render the rest of the UI
    // Calculate score position with sine wave motion
    float scoreY = SCREEN_TOP - 0.1f + 
                  SCORE_BOB_AMOUNT * sin(scoreAnimTime * SCORE_BOB_SPEED);
    
    // Render score text
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Current score
    glRasterPos2f(SCREEN_LEFT_GAMEPLAY + 0.1f, scoreY);
    
    char scoreText[32];
    sprintf(scoreText, "YOUR SCORE: %d", currentScore);
    for (const char* c = scoreText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
    
    // High score (positioned below current score)
    glRasterPos2f(SCREEN_LEFT_GAMEPLAY + 0.1f, scoreY - 0.05f);
    char highScoreText[32];
    sprintf(highScoreText, "HIGH SCORE: %d", highScore);
    for (const char* c = highScoreText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
	
    // Don't render hp bar UI if player is dying
    if (player->isDying()) {
        return;
    }

    // Constants for bar positioning and size
    const float BAR_WIDTH = 0.01f;        
    const float BAR_HEIGHT = 0.15f;    
	  const float CORNER_RADIUS = 0.025f;    
    const int SEGMENTS = 12;        
    
    // Offsets from player position
    const float BAR_OFFSET_X = 0.0f;       
    const float BAR_OFFSET_Y = 0.05f;                

    // Enable alpha blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Calculate health percentage for display
    float healthPercentage = displayedHealth / maxPlayerHealth;
    float currentBarHeight = BAR_HEIGHT * healthPercentage;
    
    // Render background (semi-transparent grey)
    glColor4f(0.7f, 0.7f, 0.7f, 0.9f);
    glBegin(GL_POLYGON);
    
    // Draw bottom half-circle
    for (int i = 0; i <= SEGMENTS/2; i++) {
        float angle = M_PI + (M_PI * i / (SEGMENTS/2));  // Start from left (pi to 2pi)
        float posX = x + BAR_OFFSET_X + CORNER_RADIUS * cos(angle);
        float posY = y - BAR_HEIGHT/2 + CORNER_RADIUS * sin(angle);
        glVertex2f(posX, posY);
    }
    
    // Draw top half-circle
    for (int i = 0; i <= SEGMENTS/2; i++) {
        float angle = (M_PI * i / (SEGMENTS/2));  // 0 to pi
        float posX = x + BAR_OFFSET_X + CORNER_RADIUS * cos(angle);
        float posY = y + BAR_HEIGHT/2 + CORNER_RADIUS * sin(angle);
        glVertex2f(posX, posY);
    }
    glEnd();
    
    // Render health bar with gradient
    glBegin(GL_POLYGON);
    float r = healthPercentage < 0.5f ? 1.0f : 2.0f * (1.0f - healthPercentage);
    float g = healthPercentage > 0.5f ? 1.0f : 2.0f * healthPercentage;
    glColor4f(r, g, 0.0f, 0.9f);
    
    // Draw bottom half-circle
    for (int i = 0; i <= SEGMENTS/2; i++) {
        float angle = M_PI + (M_PI * i / (SEGMENTS/2));
        float posX = x + BAR_OFFSET_X + CORNER_RADIUS * cos(angle);
        float posY = y - BAR_HEIGHT/2 + CORNER_RADIUS * sin(angle);
        glVertex2f(posX, posY);
    }
    
    // Draw top half-circle
    for (int i = 0; i <= SEGMENTS/2; i++) {
        float angle = (M_PI * i / (SEGMENTS/2));
        float posX = x + BAR_OFFSET_X + CORNER_RADIUS * cos(angle);
        float posY = y - BAR_HEIGHT/2 + currentBarHeight + CORNER_RADIUS * sin(angle);
        glVertex2f(posX, posY);
    }
    glEnd();
    
    glColor4f(0.9f, 0.9f, 0.9f, 1.0f); 
    glBegin(GL_POLYGON);
    for (int i = 0; i <= SEGMENTS; i++) {
        float angle = 2.0f * M_PI * i / SEGMENTS;
        float posX = x + BAR_OFFSET_X + CORNER_RADIUS * cos(angle);
        float posY = y - BAR_HEIGHT/2 + CORNER_RADIUS * sin(angle);
        glVertex2f(posX, posY);
    }
    glEnd();

    glDisable(GL_BLEND);
}

void UI::update(float deltaTime) {       
    // Smoothly animate the displayed health towards the actual health
    float diff = playerHealth - displayedHealth;
    
    if (abs(diff) > 0.1f) {
        displayedHealth += diff * animationSpeed * deltaTime;
    } else {
        displayedHealth = playerHealth;
    }

    scoreAnimTime += deltaTime;

    if (isGameOver) {
        if (gameOver) {
            gameOver->update(deltaTime);
        } else {
            std::cerr << "ERROR: gameOver pointer is null!" << std::endl;
        }
    }
}

void UI::setHealth(float health) {
    playerHealth = std::max(0.0f, std::min(health, maxPlayerHealth));
}

void UI::damage(float amount) {
    setHealth(playerHealth - amount);
    player->takeDamage();
    if (playerHealth <= 0) {
        isGameOver = true;
        gameOver->setScores(currentScore, highScore);
        player->startDeathSequence();
    }
}

void UI::heal(float amount) {
    setHealth(playerHealth + amount);
}

void UI::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
} 

void UI::addScore(int amount) {
    currentScore += amount;
    if (currentScore > highScore) {
        highScore = currentScore;
        saveHighScore();
    }
}

void UI::loadHighScore() {
    std::ifstream file(HIGH_SCORE_FILE, std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));
        file.close();
    } else {
        highScore = 0;
    }
}

void UI::saveHighScore() {
    std::ofstream file(HIGH_SCORE_FILE, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&highScore), sizeof(highScore));
        file.close();
    }
}

void UI::restart() {
    if (isGameOver) {
        // Reset score
        currentScore = 0;
        
        // Reset player
        player->reset();
        
        // Reset game over state
        isGameOver = false;
        
        // Reset health
        playerHealth = maxPlayerHealth;
        displayedHealth = maxPlayerHealth;
    }
}
