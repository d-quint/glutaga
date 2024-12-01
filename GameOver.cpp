#include "GameOver.h"

// Pixel art patterns for each letter
const bool GameOver::LETTER_G[8][8] = {
    {0,0,1,1,1,1,1,0},
    {0,1,1,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,1,1,1,0},
    {1,1,0,0,0,0,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}
};

const bool GameOver::LETTER_A[8][8] = {
    {0,0,1,1,1,0,0,0},
    {0,1,1,0,1,1,0,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {1,1,1,1,1,1,1,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {0,0,0,0,0,0,0,0}
};

const bool GameOver::LETTER_M[8][8] = {
    {1,1,0,0,0,1,1,0},
    {1,1,1,0,1,1,1,0},
    {1,1,1,1,1,1,1,0},
    {1,1,0,1,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {0,0,0,0,0,0,0,0}
};

const bool GameOver::LETTER_E[8][8] = {
    {1,1,1,1,1,1,1,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,1,1,1,1,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}
};

const bool GameOver::LETTER_O[8][8] = {
    {0,1,1,1,1,1,0,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {0,1,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,0}
};

const bool GameOver::LETTER_V[8][8] = {
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {0,1,1,0,1,1,0,0},
    {0,0,1,1,1,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

const bool GameOver::LETTER_R[8][8] = {
    {1,1,1,1,1,1,0,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,1,1,1,0},
    {1,1,1,1,1,0,0,0},
    {1,1,0,1,1,1,0,0},
    {1,1,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0}
};

GameOver::GameOver() : scoreY(0.3f), animTime(0), currentScore(0), highScore(0), fadeAlpha(0.0f) {
}

float GameOver::calculateCenteredX() const {
    // Calculate actual letter width (8 pixels * PIXEL_SIZE)
    float letterWidth = 8 * PIXEL_SIZE;
    
    // Total width calculation for "GAME OVER"
    float totalWidth = (4 * (letterWidth + LETTER_SPACING) - LETTER_SPACING) + 
                      WORD_SPACING +
                      (4 * (letterWidth + LETTER_SPACING));
    
    // Center position - using regular screen boundaries
    return (SCREEN_LEFT + SCREEN_RIGHT - totalWidth) / 2.0f;
}

void GameOver::renderPixelLetter(const bool letter[8][8], float offsetX, float baseY) {
    glBegin(GL_QUADS);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(letter[i][j]) {
                float pixelX = offsetX + (j * PIXEL_SIZE);
                float pixelY = baseY + (7-i) * PIXEL_SIZE;
                
                glVertex2f(pixelX, pixelY);
                glVertex2f(pixelX + PIXEL_SIZE, pixelY);
                glVertex2f(pixelX + PIXEL_SIZE, pixelY + PIXEL_SIZE);
                glVertex2f(pixelX, pixelY + PIXEL_SIZE);
            }
        }
    }
    glEnd();
}

void GameOver::renderCenteredText(const char* text, float y) {
    float charWidth = 0.0222f;
    float textWidth = strlen(text) * charWidth;
    float x = (SCREEN_LEFT + SCREEN_RIGHT - textWidth) / 2.0f;
    
    glRasterPos2f(x, y);
    for(const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
}

void GameOver::render() {
    // Render semi-transparent black overlay first
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(0.0f, 0.0f, 0.0f, fadeAlpha);
    glBegin(GL_QUADS);
        glVertex2f(SCREEN_LEFT_GAMEPLAY, SCREEN_TOP);
        glVertex2f(SCREEN_RIGHT_GAMEPLAY, SCREEN_TOP);
        glVertex2f(SCREEN_RIGHT_GAMEPLAY, SCREEN_BOTTOM);
        glVertex2f(SCREEN_LEFT_GAMEPLAY, SCREEN_BOTTOM);
    glEnd();
    
    glDisable(GL_BLEND);

    // Only render game over text when fade is partially visible
    if (fadeAlpha > 0.1f) {
        // Calculate bobbing position
        float renderY = 0.2f + BOB_AMOUNT * sin(animTime * BOB_SPEED);
        float scoreRenderY = renderY - SCORE_OFFSET;
        
        // Set white color for text
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        
        // Calculate centered starting X position
        float startX = calculateCenteredX();
        float letterWidth = 8 * PIXEL_SIZE;
        
        // Render "GAME"
        renderPixelLetter(LETTER_G, startX, renderY);
        renderPixelLetter(LETTER_A, startX + letterWidth + LETTER_SPACING, renderY);
        renderPixelLetter(LETTER_M, startX + (letterWidth + LETTER_SPACING) * 2, renderY);
        renderPixelLetter(LETTER_E, startX + (letterWidth + LETTER_SPACING) * 3, renderY);
        
        // Calculate start position for "OVER" (after WORD_SPACING)
        float overStartX = startX + (letterWidth + LETTER_SPACING) * 4 + WORD_SPACING;
        
        // Render "OVER"
        renderPixelLetter(LETTER_O, overStartX, renderY);
        renderPixelLetter(LETTER_V, overStartX + letterWidth + LETTER_SPACING, renderY);
        renderPixelLetter(LETTER_E, overStartX + (letterWidth + LETTER_SPACING) * 2, renderY);
        renderPixelLetter(LETTER_R, overStartX + (letterWidth + LETTER_SPACING) * 3, renderY);
        
        // Prepare score texts
        char scoreText[32];
        char highScoreText[32];
        sprintf(scoreText, "FINAL SCORE: %d", currentScore);
        sprintf(highScoreText, "HIGH SCORE: %d", highScore);
        
        // Render centered texts with consistent spacing
        renderCenteredText(scoreText, scoreRenderY);
        renderCenteredText(highScoreText, scoreRenderY - TEXT_LINE_SPACING);
        
        // Show instruction texts
        renderCenteredText("Press R to Restart", scoreRenderY - TEXT_LINE_SPACING * 3);
        renderCenteredText("Press ESC to Exit", scoreRenderY - TEXT_LINE_SPACING * 4);
        
        // Show credits at the bottom of the screen
        renderCenteredText("GLUTAGA is brought to you by Dane Quintano, Jansen Moral, JC Paglinawan, & Dharmveer Sandhu of TN34", scoreRenderY - TEXT_LINE_SPACING * 18);
    }
}

void GameOver::update(float deltaTime) {
    animTime += deltaTime;
    
    // Update fade alpha
    if (fadeAlpha < FINAL_FADE_ALPHA) {
        fadeAlpha += (FINAL_FADE_ALPHA / FADE_DURATION) * deltaTime;
        if (fadeAlpha > FINAL_FADE_ALPHA) {
            fadeAlpha = FINAL_FADE_ALPHA;
        }
    }
}

void GameOver::setScores(int score, int high) {
    currentScore = score;
    highScore = high;
} 
