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

// Screen boundaries
const float SCREEN_LEFT = -1.0f;
const float SCREEN_RIGHT = 1.0f;
const float SCREEN_TOP = 1.0f;
const float SCREEN_BOTTOM = -1.0f;

// Player and projectile properties
const float BOUNCE_FACTOR = 0.7f;
const float PROJECTILE_SPEED = 0.05f;
const float PROJECTILE_WIDTH = 0.02f;
const float PROJECTILE_HEIGHT = 0.15f;
const int MAX_PROJECTILES = 100;
const int MAX_ENEMY_PROJECTILES = 200;
const int MAX_ENEMIES = 30;

// Shared input states
extern bool keyStates[256];
extern bool specialKeyStates[256];
extern bool mouseButtons[3];
extern bool leftMouseWasPressed; 

#endif // GLOBALS_H
