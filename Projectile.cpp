#define GLEW_STATIC

#include "Projectile.h"

Projectile::Projectile(bool _fromEnemy) : x(0), y(0), velocityY(PROJECTILE_SPEED), active(false), fromEnemy(_fromEnemy) {
    initializeBuffers();
}

Projectile::~Projectile() {
    delete[] vertices;
    delete[] colors;
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboColors);
}

void Projectile::initializeBuffers() {
    // Generate vertex buffer objects
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboColors);

    // Allocate arrays
    vertices = new float[8];  // 4 vertices * 2 coordinates
    colors = new float[16];   // 4 vertices * 4 colors (RGBA)

    // Initialize vertex data for a rectangle
    updateVertices();

    // Initialize color data with gradients
    if (fromEnemy) {
        // Enemy projectile: White with green glow
        colors[0] = 1.0f;   // White core
        colors[1] = 1.0f;
        colors[2] = 1.0f;
        colors[3] = 1.0f;   // Full opacity

        colors[4] = 1.0f;
        colors[5] = 1.0f;
        colors[6] = 1.0f;
        colors[7] = 1.0f;

        colors[8] = 0.2f;   // Green glow
        colors[9] = 1.0f;
        colors[10] = 0.2f;
        colors[11] = 0.0f;  // Transparent edge

        colors[12] = 0.2f;
        colors[13] = 1.0f;
        colors[14] = 0.2f;
        colors[15] = 0.0f;
    } else {
        // Player projectile: White with blue glow
        colors[0] = 1.0f;    // Orange glow
        colors[1] = 0.7f; 
        colors[2] = 0.0f;
        colors[3] = 0.0f;   // Transparent edge

        colors[4] = 1.0f; 
        colors[5] = 0.7f; 
        colors[6] = 0.0f;
        colors[7] = 0.0f;

        colors[8] = 1.0f;   // White core
        colors[9] = 1.0f;
        colors[10] = 1.0f;
        colors[11] = 1.0f;  // Full opacity

        colors[12] = 1.0f;
        colors[13] = 1.0f;
        colors[14] = 1.0f;
        colors[15] = 1.0f;
    }

    // Bind and fill vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    // Bind and fill color buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), colors, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Projectile::updateVertices() {
   vertices[0] = x - PROJECTILE_WIDTH/2; vertices[1] = y - PROJECTILE_HEIGHT/2;  
   vertices[2] = x + PROJECTILE_WIDTH/2; vertices[3] = y - PROJECTILE_HEIGHT/2; 
   vertices[4] = x + PROJECTILE_WIDTH/2; vertices[5] = y + PROJECTILE_HEIGHT/2;  
   vertices[6] = x - PROJECTILE_WIDTH/2; vertices[7] = y + PROJECTILE_HEIGHT/2;  
}

void Projectile::spawn(float startX, float startY) {
    x = startX;
    y = startY;
    active = true;
    updateVertices();

    // Update VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), vertices);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Projectile::update() {
    if (!active)
        return;

	if (!fromEnemy) {
		y += velocityY;
		
		// Deactivate if out of bounds
	    if (y > SCREEN_TOP) {
	        active = false;
	        return;
	    }
	} else {
    	y -= velocityY;
		
		// Deactivate if out of bounds
	    if (y < SCREEN_BOTTOM) {
	        active = false;
	        return;
	    }
    }

    updateVertices();

    // Update VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), vertices);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Projectile::render() {
    if (!active)
        return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableVertexAttribArray(0); 

    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glColorPointer(4, GL_FLOAT, 0, 0);

    glDrawArrays(GL_QUADS, 0, 4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisable(GL_BLEND);
}

void Projectile::deactivate() { active = false; }

// Getters
float Projectile::getX() const { return x; }
float Projectile::getY() const { return y; }
bool Projectile::isActive() const { return active; }
