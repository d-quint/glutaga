#include "Input.h"
#include "Globals.h"

void keyboardDown(unsigned char key, int x, int y) {
    keyStates[key] = true;
    
    // Check for ESC key (ASCII value 27)
    if (key == 27) {
        glutLeaveMainLoop();  // This will cleanly exit the program
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void specialKeyDown(int key, int x, int y) {
    specialKeyStates[key] = true;
}

void specialKeyUp(int key, int x, int y) {
    specialKeyStates[key] = false;
}

void mouseButton(int button, int state, int x, int y) {
    mouseButtons[button] = (state == GLUT_DOWN);
}
