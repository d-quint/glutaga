#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <vector>
#include <windows.h>

// Screen boundaries
const float SCREEN_LEFT = -1.0f;
const float SCREEN_RIGHT = 1.0f;
const float SCREEN_TOP = 1.0f;
const float SCREEN_BOTTOM = -1.0f;

// Player's bounce coefficient (1.0 = perfect bounce, 0.0 = full stop)
const float BOUNCE_FACTOR = 0.7f;

// Projectile properties
const float PROJECTILE_SPEED = 0.05f;
const float PROJECTILE_WIDTH = 0.01f;
const float PROJECTILE_HEIGHT = 0.08f;
const int MAX_PROJECTILES = 100;

bool keyStates[256] = { false };  // For regular keys
bool specialKeyStates[256] = { false };  // For special keys

void keyboardDown(unsigned char key, int x, int y) {
    keyStates[key] = true;
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

class Projectile {
private:
    float x, y;
    float velocityY;
    bool active;
    
    // VBO data
    GLuint vboVertices;
    GLuint vboColors;
    float* vertices;
    float* colors;

    void initializeBuffers() {
        // Generate vertex buffer objects
        glGenBuffers(1, &vboVertices);
        glGenBuffers(1, &vboColors);

        // Allocate arrays
        vertices = new float[8];  // 4 vertices * 2 coordinates
        colors = new float[12];   // 4 vertices * 3 colors (RGB)

        // Initialize vertex data for a rectangle
        updateVertices();

        // Initialize color data (yellow projectile)
        for (int i = 0; i < 6; i += 3) {
            colors[i] = 0.0f;     // R
            colors[i+1] = 0.0f;   // G
            colors[i+2] = 0.0f;   // B
        }
        
        for (int i = 6; i < 12; i += 3) {
            colors[i] = 1.0f;     // R
            colors[i+1] = 1.0f;   // G
            colors[i+2] = 0.0f;   // B
        }

        // Bind and fill vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

        // Bind and fill color buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboColors);
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STATIC_DRAW);
    }

    void updateVertices() {
        vertices[0] = x - PROJECTILE_WIDTH/2; vertices[1] = y - PROJECTILE_HEIGHT/2;  // Bottom left
        vertices[2] = x + PROJECTILE_WIDTH/2; vertices[3] = y - PROJECTILE_HEIGHT/2;  // Bottom right
        vertices[4] = x + PROJECTILE_WIDTH/2; vertices[5] = y + PROJECTILE_HEIGHT/2;  // Top right
        vertices[6] = x - PROJECTILE_WIDTH/2; vertices[7] = y + PROJECTILE_HEIGHT/2;  // Top left
    }

public:
    Projectile() : active(false), velocityY(PROJECTILE_SPEED) {
        vertices = NULL;
        colors = NULL;
        initializeBuffers();
    }

    ~Projectile() {
        delete[] vertices;
        delete[] colors;
        glDeleteBuffers(1, &vboVertices);
        glDeleteBuffers(1, &vboColors);
    }

    void spawn(float startX, float startY) {
        x = startX;
        y = startY;
        active = true;
        updateVertices();
        
        // Update VBO
        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), vertices);
    }

    void update() {
        if (!active) return;

        y += velocityY;
        
        // Deactivate if out of bounds
        if (y > SCREEN_TOP) {
            active = false;
            return;
        }

        updateVertices();
        
        // Update VBO
        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), vertices);
    }

    void render() {
        if (!active) return;

        glEnableVertexAttribArray(0);  // Position

        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vboColors);
        glColorPointer(3, GL_FLOAT, 0, 0);

        glDrawArrays(GL_QUADS, 0, 4);

        glDisableVertexAttribArray(0);
    }

    bool isActive() const { return active; }
};

class Player {
private:
    // Position and movement
    float x, y;
    float velocityX, velocityY;
    float acceleration;
    float friction;
    float maxSpeed;
    float size;
    
    // Vertex and color arrays
    float* vertices;
    float* colors;
    
    // Buffer objects
    GLuint vboVertices;
    GLuint vboColors;

    void initializeBuffers() {
        // Generate vertex buffer objects
        glGenBuffers(1, &vboVertices);
        glGenBuffers(1, &vboColors);

        // Allocate arrays
        vertices = new float[8];  // 4 vertices * 2 coordinates
        colors = new float[12];   // 4 vertices * 3 colors (RGB)

        // Initialize vertex data for a square
        vertices[0] = -size; vertices[1] = -size;  // Bottom left
        vertices[2] = size;  vertices[3] = -size;  // Bottom right
        vertices[4] = size;  vertices[5] = size;   // Top right
        vertices[6] = -size; vertices[7] = size;   // Top left

        // Initialize color data (white for now)
        for (int i = 0; i < 12; i += 3) {
            colors[i] = 1.0f;     // R
            colors[i+1] = 1.0f;   // G
            colors[i+2] = 0.0f;   // B
        }
        
        // Bind and fill vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_STATIC_DRAW);

        // Bind and fill color buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboColors);
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STATIC_DRAW);
    }

    void checkBoundaryCollision() {
        // Right wall
        if (x + size > SCREEN_RIGHT) {
            x = SCREEN_RIGHT - size;
            velocityX = -velocityX * BOUNCE_FACTOR;
        }
        // Left wall
        if (x - size < SCREEN_LEFT) {
            x = SCREEN_LEFT + size;
            velocityX = -velocityX * BOUNCE_FACTOR;
        }
        // Top wall
        if (y + size > SCREEN_TOP) {
            y = SCREEN_TOP - size;
            velocityY = -velocityY * BOUNCE_FACTOR;
        }
        // Bottom wall
        if (y - size < SCREEN_BOTTOM) {
            y = SCREEN_BOTTOM + size;
            velocityY = -velocityY * BOUNCE_FACTOR;
        }
    }

public:
    Player(float startX = 0.0f, float startY = 0.0f, float playerSize = 0.1f) 
        : x(startX), y(startY), size(playerSize) {
        // Initialize physics parameters
        velocityX = 0.0f;
        velocityY = 0.0f;
        acceleration = 0.001f;    // Acceleration when key is pressed
        friction = 0.98f;         // Friction coefficient (1.0 = no friction)
        maxSpeed = 0.02f;         // Maximum speed cap
        
        vertices = NULL;
        colors = NULL;
        initializeBuffers();
    }

    ~Player() {
        delete[] vertices;
        delete[] colors;
        glDeleteBuffers(1, &vboVertices);
        glDeleteBuffers(1, &vboColors);
    }

    void update() {
        // Apply friction
        velocityX *= friction;
        velocityY *= friction;
        
        // Update position based on velocity
        x += velocityX;
        y += velocityY;
        
        // Check for wall collisions
        checkBoundaryCollision();
        
        // Update vertex positions
        updatePosition();
        
        // Stop completely if moving very slowly
        if (std::abs(velocityX) < 0.0001f) velocityX = 0;
        if (std::abs(velocityY) < 0.0001f) velocityY = 0;
    }

    void updatePosition() {
        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
        float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        
        if (ptr) {
            ptr[0] = x - size; ptr[1] = y - size;  // Bottom left
            ptr[2] = x + size; ptr[3] = y - size;  // Bottom right
            ptr[4] = x + size; ptr[5] = y + size;  // Top right
            ptr[6] = x - size; ptr[7] = y + size;  // Top left
            
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
    }

    void render() {
        glEnableVertexAttribArray(0);  // Position

        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vboColors);
        glColorPointer(3, GL_FLOAT, 0, 0);

        glDrawArrays(GL_QUADS, 0, 4);

        glDisableVertexAttribArray(0);
    }

    void moveLeft() {
        velocityX -= acceleration;
        if (velocityX < -maxSpeed) velocityX = -maxSpeed;
    }

    void moveRight() {
        velocityX += acceleration;
        if (velocityX > maxSpeed) velocityX = maxSpeed;
    }

    void moveUp() {
        velocityY += acceleration;
        if (velocityY > maxSpeed) velocityY = maxSpeed;
    }

    void moveDown() {
        velocityY -= acceleration;
        if (velocityY < -maxSpeed) velocityY = -maxSpeed;
    }

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getSize() const { return size; }
};

Player* player;

std::vector<Projectile*> projectiles;
bool spaceWasPressed = false;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    player->render();
    
    // Render all active projectiles
	for(int i = 0; i < projectiles.size(); i++) {
	    projectiles[i]->render();
	}

    glFlush();
}

void mouse(int button, int state, int x, int y) {

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
	    for(int i = 0; i < projectiles.size(); i++) {
	        if (!projectiles[i]->isActive()) {
	            projectiles[i]->spawn(player->getX(), player->getY() + player->getSize());
	            break;
	        }
	    }
	    spaceWasPressed = true;
	}
    else if (!keyStates[' ']) {
        spaceWasPressed = false;
    }
}

void update(int value) {
	processInput();
	player->update();
	
	// Update all active projectiles
	for(int i = 0; i < projectiles.size(); i++) {
	    projectiles[i]->update();
	}
	
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void initObjects() {
	glEnableClientState(GL_COLOR_ARRAY);
	
	player = new Player();
	
	// Initialize projectile pool
    for(int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles.push_back(new Projectile());
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("project");
    
    glutDisplayFunc(display);

	glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);

    glutMouseFunc(mouse);
    glutTimerFunc(0, update, 0);
    
    GLenum err = glewInit();
    
    if (err == GLEW_OK) {
        initObjects();
        glutMainLoop();
    }
    
    return 0;
}
