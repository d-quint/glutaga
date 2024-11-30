#ifndef GLOBALS_H
#define GLOBALS_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Screen boundaries
const float SCREEN_LEFT = -1.0f;
const float SCREEN_RIGHT = 1.0f;
const float SCREEN_TOP = 1.0f;
const float SCREEN_BOTTOM = -1.0f;

// Player and projectile properties
const float BOUNCE_FACTOR = 0.7f;
const float PROJECTILE_SPEED = 0.05f;
const float PROJECTILE_WIDTH = 0.0075f;
const float PROJECTILE_HEIGHT = 0.2f;
const int MAX_PROJECTILES = 100;
const int MAX_ENEMY_PROJECTILES = 100;
const int MAX_ENEMIES = 30;

// Shared input states
extern bool keyStates[256];
extern bool specialKeyStates[256];
extern bool mouseButtons[3];
extern bool leftMouseWasPressed; 

// For viewport tracking & aspect ratio
extern int windowWidth;
extern int windowHeight;
extern float viewportX;
extern float viewportY;
extern float viewportWidth;
extern float viewportHeight;

extern float GAME_ASPECT_RATIO;
extern float SCREEN_LEFT_GAMEPLAY;
extern float SCREEN_RIGHT_GAMEPLAY;

#endif // GLOBALS_H
