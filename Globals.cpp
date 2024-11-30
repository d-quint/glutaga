#include "Globals.h"

bool keyStates[256] = { false };
bool specialKeyStates[256] = { false };
bool mouseButtons[3] = { false };
bool leftMouseWasPressed = false;
int windowWidth = 0;
int windowHeight = 0;
float viewportX = 0;
float viewportY = 0;
float viewportWidth = 0;
float viewportHeight = 0;
float GAME_ASPECT_RATIO = 1.0f;
float SCREEN_LEFT_GAMEPLAY = -1.0f;
float SCREEN_RIGHT_GAMEPLAY = 1.0f;
