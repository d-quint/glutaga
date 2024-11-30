#define GLEW_STATIC

#include "Globals.h"
#include "Player.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Input.h"
#include "Background.h"
#include "UI.h"

Player* player;
Background* background;
std::vector<Projectile*> projectiles;
std::vector<Projectile*> enemyProjectiles;
std::vector<Enemy*> enemies;
UI* ui;

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
    
    // Render UI elements last (on top of everything)
    ui->render();
    
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

void mouseWheel(int wheel, int direction, int x, int y)
{
    // Loop through all enemies and adjust their size
    for (int i = 0; i < enemies.size(); i++)
    {
        float currentSize = enemies[i]->getSize();

        // Increase/decrease size based on scroll direction
        // direction will be +1 for scroll up, -1 for scroll down
        float newSize = currentSize + (direction * 0.01f);

        // Clamp size between reasonable limits
        newSize = std::max(0.05f, std::min(0.2f, newSize));

        enemies[i]->setSize(newSize);
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

    // Check enemy projectile collisions with player
    for (int i = 0; i < enemyProjectiles.size(); i++) {
        if (enemyProjectiles[i]->isActive() && 
            player->checkCollision(
                enemyProjectiles[i]->getX(), 
                enemyProjectiles[i]->getY(), 
                PROJECTILE_WIDTH, 
                PROJECTILE_HEIGHT)) {
            enemyProjectiles[i]->deactivate();
            ui->damage(10.0f);
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
    
    // Update UI position to follow player
    ui->setPosition(player->getX(), player->getY());

    updateProjectiles();
    updateSpawnEnemies();
    updateShootEnemies();
    updateEnemies();

    background->update(0.016f);
    ui->update(0.016f);

    // Check if player should die after health animation completes
    if (ui->getHealth() <= 0 && ui->isHealthAnimationComplete()) {
        player->startDeathSequence();
    }

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
    
    ui = new UI(player, 100.0f);  // Pass player pointer to UI
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

void cleanup() {
    // Clean up allocated objects
    delete player;
    delete background;
    
    // Clean up projectiles
    for (int i = 0; i < projectiles.size(); i++) {
        delete projectiles[i];
    }
    projectiles.clear();
    
    for (int i = 0; i < enemyProjectiles.size(); i++) {
        delete enemyProjectiles[i];
    }
    enemyProjectiles.clear();
    
    // Clean up enemies
    for (int i = 0; i < enemies.size(); i++) {
        delete enemies[i];
    }
    enemies.clear();
    
    delete ui;
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
    glutMouseWheelFunc(mouseWheel);
    glutTimerFunc(0, update, 0);

    glewInit();
    initObjects();
    atexit(cleanup);
    glutMainLoop();

    return 0;
}
