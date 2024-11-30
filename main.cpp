#define GLEW_STATIC

#include "Globals.h"
#include "Player.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Input.h"
#include "Background.h"

Player* player;
Background* background;
std::vector<Projectile*> projectiles;
std::vector<Projectile*> enemyProjectiles;
std::vector<Enemy*> enemies;

void display() { 
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render the background
    background->render();
    
    // Render all active projectiles
	for (int i = 0; i < projectiles.size(); i++) {
	    projectiles[i]->render();
	}
	
	for (int i = 0; i < enemyProjectiles.size(); i++) {
	    enemyProjectiles[i]->render();
	}
	
	// Render all active enemies
	for (int i = 0; i < enemies.size(); i++) {
	    enemies[i]->render();
	}
	
	player->render();

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
	if (mouseButtons[GLUT_LEFT_BUTTON] && !leftMouseWasPressed) {
	    player->shoot(projectiles);
	    leftMouseWasPressed = true;
	} else if (!mouseButtons[GLUT_LEFT_BUTTON]) {
	    leftMouseWasPressed = false;
	}
}

void updateProjectiles() {
    // Update all active projectiles
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->update();
    }
    
    for (int i = 0; i < enemyProjectiles.size(); i++) {
        enemyProjectiles[i]->update();
    }

    // Check projectile-to-enemy collisions
    for (int i = 0; i < enemies.size(); i++) {
        for (int j = 0; j < projectiles.size(); j++) {
            if (projectiles[j]->isActive() && enemies[i]->checkCollision(
                    projectiles[j]->getX(), 
                    projectiles[j]->getY(), 
                    PROJECTILE_WIDTH, 
                    PROJECTILE_HEIGHT)) {
                projectiles[j]->deactivate(); // Deactivate projectile on hit
            }
        }
    }
}

void updateSpawnEnemies() {
	static float spawnTimer = 0.0f;
	spawnTimer += 0.016f; // 60 fps
	
	if (spawnTimer > 2.0f) { 
	    for (int i = 0; i < enemies.size(); i++) {
	        if (!enemies[i]->isActive()) {
	            enemies[i]->spawn();
	            break;
	        }
	    }
	    spawnTimer = 0.0f;
	}
}

void updateShootEnemies() {
	static float shootTimer = 0.0f;
	shootTimer += 0.016f; // 60 fps
	
	if (shootTimer > 1.0f) { // Shoot every 1 second
	    for (int i = 0; i < enemies.size(); i++) {
	        if (enemies[i]->isActive()) {
	            enemies[i]->shoot(enemyProjectiles);
	        }
	    }
	    shootTimer = 0.0f;
	}
}

void updateEnemies() {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i]->update(16.0f);
    }
}

void update(int value) {
    processInput();
	
    player->update();

    updateProjectiles();
    updateSpawnEnemies();
    updateShootEnemies();
    updateEnemies();

    background->update(0.016f);

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void initObjects() { 
    glEnableClientState(GL_COLOR_ARRAY);
	
	player = new Player(0.0f, -0.5f, 0.085f);
    background = new Background();
	
	// Initialize projectile pools
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles.push_back(new Projectile(false));
    }
    
    for (int i = 0; i < MAX_ENEMY_PROJECTILES; i++) {
        enemyProjectiles.push_back(new Projectile(true));
    }
    
    // Initialize pool of enemies
    for (int i = 0; i < MAX_ENEMIES; i++) { 
	    enemies.push_back(new Enemy());
	}
}

void reshapeWindow(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    
    // Calculate aspect ratio
    float aspectRatio = (float)w / (float)h;
    GAME_ASPECT_RATIO = aspectRatio;
    
    // Update gameplay boundaries based on aspect ratio
    SCREEN_LEFT_GAMEPLAY = SCREEN_LEFT * aspectRatio;
    SCREEN_RIGHT_GAMEPLAY = SCREEN_RIGHT * aspectRatio;
    
    // Use full window as viewport
    glViewport(0, 0, w, h);
    
    // Set up orthographic projection that maintains square proportions in the center
    // but extends to fill the width
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(SCREEN_LEFT_GAMEPLAY, SCREEN_RIGHT_GAMEPLAY, SCREEN_BOTTOM, SCREEN_TOP, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // Get screen dimensions
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    
    glutCreateWindow("GLUTAGA");
    glutFullScreen();
    
    windowWidth = screenWidth;
    windowHeight = screenHeight;
    reshapeWindow(screenWidth, screenHeight);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeWindow); 
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    glutMouseFunc(mouseButton);
    glutTimerFunc(0, update, 0);

    glewInit();
    initObjects();
    glutMainLoop();

    return 0;
}
