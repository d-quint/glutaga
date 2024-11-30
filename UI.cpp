#include "UI.h"

void UI::render() {
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
}

void UI::setHealth(float health) {
    playerHealth = std::max(0.0f, std::min(health, maxPlayerHealth));
}

void UI::damage(float amount) {
    setHealth(playerHealth - amount);
}

void UI::heal(float amount) {
    setHealth(playerHealth + amount);
}

void UI::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
} 
