#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "Globals.h"

class GameOver {
private:
    float scoreY;
    float animTime;
    int currentScore;
    int highScore;
    float fadeAlpha;  // Current alpha value for fade effect
    
    static const float BOB_SPEED = 2.0f;
    static const float BOB_AMOUNT = 0.02f;
    static const float SCORE_OFFSET = 0.2f;  // Vertical offset for score text
    static const float LETTER_SPACING = 0.15f;  // Space between letters
    static const float WORD_SPACING = 0.25f;   // Space between GAME and OVER
    static const float PIXEL_SIZE = 0.02f;     // Size of each pixel in the letter grid
    static const float TEXT_LINE_SPACING = 0.05f; // Spacing between lines of text
    static const float FADE_DURATION = 1.0f;  // Time in seconds for fade effect
    static const float FINAL_FADE_ALPHA = 0.7f;  // Final alpha value for background dim
    
    // Pixel patterns for letters (8x8 grid)
    static const bool LETTER_G[8][8];
    static const bool LETTER_A[8][8];
    static const bool LETTER_M[8][8];
    static const bool LETTER_E[8][8];
    static const bool LETTER_O[8][8];
    static const bool LETTER_V[8][8];
    static const bool LETTER_R[8][8];
    
    void renderPixelLetter(const bool letter[8][8], float offsetX, float baseY);
    float calculateCenteredX() const;
    void renderCenteredText(const char* text, float y);
    
public:
    GameOver();
    void render();
    void update(float deltaTime);
    void setScores(int score, int high);
    void reset();
};

#endif // GAMEOVER_H
