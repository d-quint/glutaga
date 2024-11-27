#define GLEW_STATIC

#include "Globals.h"
#include "Player.h"
#include "Projectile.h"
#include "Input.h"

Player* player;
std::vector<Projectile*> projectiles;

void display() { 
    glClear(GL_COLOR_BUFFER_BIT);
    
    player->render();
    
    // Render all active projectiles
	for(int i = 0; i < projectiles.size(); i++) {
	    projectiles[i]->render();
	}

    glFlush();
}

void processInput() {
    // Check arrow keys and WASD keys
    if (specialKeyStates[GLUT_KEY_LEFT] || keyStates['a'])
        player->moveLeft();
    if (specialKeyStates[GLUT_KEY_RIGHT] || keyStates['d'])
        player->moveRight();
    if (specialKeyStates[GLUT_KEY_UP] || keyStates['w'])
        player->moveUp();
    if (specialKeyStates[GLUT_KEY_DOWN] || keyStates['s'])
        player->moveDown();
        
    // Handle shooting
	if (keyStates[' '] && !spaceWasPressed) {
	    for (int i = 0; i < projectiles.size(); i++) {
	        if (!projectiles[i]->isActive()) {
	            projectiles[i]->spawn(player->getX(), player->getY() + player->getSize());
	            break;
	        }
	    }
	    spaceWasPressed = true;
	} else if (!keyStates[' ']) {
        spaceWasPressed = false;
    }
}

void update(int value) {
    processInput();
	player->update();
	
	// Update all active projectiles
	for (int i = 0; i < projectiles.size(); i++) {
	    projectiles[i]->update();
	}
	
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void initObjects() { 
    glEnableClientState(GL_COLOR_ARRAY);
	
	player = new Player();
	
	// Initialize projectile pool
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles.push_back(new Projectile());
    }
}

int main(int argc, char** argv) {
    // Initialize OpenGL, GLEW, and GLUT
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Game");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    glutTimerFunc(0, update, 0);

    glewInit();
    initObjects();
    glutMainLoop();

    return 0;
}
