#define GLEW_STATIC

#include "Player.h"
#include "Projectile.h"

GLfloat y_offset = -0.085f;
GLfloat outline_offset = -0.2f;  // - value to make outline large
GLfloat outline_thickness = 3.0f;

GLfloat headVertices[] = {
    // Main shape vertices
    0.0f, 0.65f + y_offset,
    0.65f, 0.0f + y_offset,
    0.0f, -0.65f + y_offset,
    -0.65f, 0.0f + y_offset,
    
    // Outline vertices (scaled inward by outline_offset)
    0.0f, (0.65f - outline_offset) + y_offset,
    (0.65f - outline_offset), 0.0f + y_offset,
    0.0f, (-0.65f + outline_offset) + y_offset,
    (-0.65f + outline_offset), 0.0f + y_offset
};

GLfloat headColors[] = {
    0.8f, 0.3f, 0.1f, 
    0.8f, 0.3f, 0.1f, 
    0.8f, 0.3f, 0.1f, 
    0.8f, 0.3f, 0.1f, 
    
    0.9f, 0.8f, 0.5f, 
    0.9f, 0.8f, 0.5f, 
    0.9f, 0.8f, 0.5f, 
    0.9f, 0.8f, 0.5f, 
};

Player::Player(float startX, float startY, float _playerSize)
    : x(startX), y(startY), playerSize(_playerSize), velocityX(0), velocityY(0),
      acceleration(0.001f), friction(0.98f), maxSpeed(0.02f),
      vertices(NULL), colors(NULL), vertexCount(0),
      isRotating(false), rotationAngle(0.0f), rotationSpeed(3.0f),
      particleSystem(60.0f) {  // 60 particles per second
    loadSpriteData("player.txt");

    // Initialize head colors
    currentHeadColor[0] = 0.8f; // Initial orange color
    currentHeadColor[1] = 0.3f;
    currentHeadColor[2] = 0.1f;

    targetHeadColor[0] = 0.8f; // Target orange color
    targetHeadColor[1] = 0.3f;
    targetHeadColor[2] = 0.1f;
}

Player::~Player() {
    delete[] vertices;
    delete[] colors;
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboColors);
    glDeleteBuffers(1, &vboHeadVertices);
    glDeleteBuffers(1, &vboHeadColors);
}

void Player::startRotation() {
    if (!isRotating) {
        isRotating = true;  // Start the rotation
        rotationAngle = 0;  // Reset rotation angle
    }
}

void Player::loadSpriteData(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open sprite file: " << filename << std::endl;
        return;
    }

    std::vector<float> tempVertices;
    std::vector<float> tempColors;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        float x, y, r, g, b;
        if (ss >> x >> y >> r >> g >> b) {
            tempVertices.push_back(x*2.0f);
            tempVertices.push_back(y*2.0f);
            tempColors.push_back(r);
            tempColors.push_back(g);
            tempColors.push_back(b);
        }
    }

    // Calculate vertex count (number of xy pairs)
    vertexCount = tempVertices.size() / 2;

    if (vertexCount * 2 != tempVertices.size() || vertexCount * 3 != tempColors.size()) {
        std::cerr << "Mismatch between vertex count and color count in sprite file" << std::endl;
        return;
    }

    // Allocate arrays
    vertices = new float[vertexCount * 2];  // xy pairs
    colors = new float[vertexCount * 3];    // rgb triplets

    // Copy data into the arrays
    std::copy(tempVertices.begin(), tempVertices.end(), vertices);
    std::copy(tempColors.begin(), tempColors.end(), colors);

    // Initialize the buffers
    initializeBuffers();

    file.close();
}

void Player::initializeBuffers() {
    if (!vertices || !colors || vertexCount == 0) {
        std::cerr << "Error: Sprite data is not loaded properly!" << std::endl;
        if (!vertices) {
            std::cerr << "Error: vertices array is NULL!" << std::endl;
        }
        if (!colors) {
            std::cerr << "Error: colors array is NULL!" << std::endl;
        }
        if (vertexCount == 0) {
            std::cerr << "Error: vertexCount is 0!" << std::endl;
        }
        return;
    }
    
    std::cout << "Sprite data loaded successfully!" << std::endl;
    std::cout << "Number of vertices: " << vertexCount << std::endl;
    std::cout << "Number of colors: " << vertexCount << std::endl; // Assuming colors match vertices

    // Generate vertex buffer objects
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboColors);
    glGenBuffers(1, &vboHeadVertices);
    glGenBuffers(1, &vboHeadColors);

    // Bind and fill vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboHeadVertices);
    glBufferData(GL_ARRAY_BUFFER, 8 * 2 * sizeof(float), headVertices, GL_DYNAMIC_DRAW);

    // Bind and fill color buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), colors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vboHeadColors);
    glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), headColors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Player::updatePosition() {
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    GLfloat* ptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    if (ptr) {
        for (size_t i = 0; i < vertexCount; i++) {
            // Update x coordinate
            ptr[i * 2] = vertices[i * 2] + x;
            // Update y coordinate
            ptr[i * 2 + 1] = vertices[i * 2 + 1] + y;
        }
        
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, vboHeadVertices);
    ptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    if (ptr) {
        for (size_t i = 0; i < 8; i++) {
            // Update x coordinate
            ptr[i * 2] = headVertices[i * 2] + x;
            // Update y coordinate
            ptr[i * 2 + 1] = headVertices[i * 2 + 1] + y;
        }
        
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Player::render() {
    glEnableVertexAttribArray(0); 
    
    // Render smoke particles first
    particleSystem.render();
    
    // Render the head fill
    glPushMatrix();  
    
    glTranslatef(x, y+(y_offset), 0.0f);
    glScalef(playerSize, playerSize, 1.0f);  // Scale head using the playerSize variable
    
    if (isRotating) {
        glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    }
    
    glTranslatef(-x, -(y+(y_offset)), 0.0f);

    // Update head color buffer with current head color
    glBindBuffer(GL_ARRAY_BUFFER, vboHeadColors);
    GLfloat updatedHeadColors[12] = {
        currentHeadColor[0], currentHeadColor[1], currentHeadColor[2],
        currentHeadColor[0], currentHeadColor[1], currentHeadColor[2],
        currentHeadColor[0], currentHeadColor[1], currentHeadColor[2],
        currentHeadColor[0], currentHeadColor[1], currentHeadColor[2]
    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(updatedHeadColors), updatedHeadColors);

    // Draw the head fill
    glBindBuffer(GL_ARRAY_BUFFER, vboHeadVertices);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboHeadColors);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glDrawArrays(GL_QUADS, 0, 4);

    glPopMatrix();
    
    // Render ship body
    glPushMatrix(); 
    
    glTranslatef(x, y, 0.0f);
    glScalef(playerSize, playerSize, 1.0f);  // Scale ship using the playerSize variable
    glTranslatef(-x, -y, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glDrawArrays(GL_QUADS, 0, vertexCount);
    
    glPopMatrix();
    
    // Render the head outline
    glPushMatrix();  
    
    glTranslatef(x, y+(y_offset), 0.0f);
    glScalef(playerSize, playerSize, 1.0f);  // Scale head using the playerSize variable
    
    if (isRotating) {
        glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    }
    
    glTranslatef(-x, -(y+(y_offset)), 0.0f);

    // Draw the head outline
    glBindBuffer(GL_ARRAY_BUFFER, vboHeadVertices);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboHeadColors);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glLineWidth(outline_thickness);  
    glDrawArrays(GL_LINE_LOOP, 4, 4);

    // Reset line width back to default
    glLineWidth(1.0f);

    glPopMatrix();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0); 
}

void Player::checkBoundaryCollision() {
    // Right wall
    if (x + playerSize > SCREEN_RIGHT_GAMEPLAY) {
        x = SCREEN_RIGHT_GAMEPLAY - playerSize;
        velocityX = -velocityX * BOUNCE_FACTOR;
    }
    // Left wall
    if (x - playerSize < SCREEN_LEFT_GAMEPLAY) {
        x = SCREEN_LEFT_GAMEPLAY + playerSize;
        velocityX = -velocityX * BOUNCE_FACTOR;
    }
    // Top wall
    if (y + playerSize > SCREEN_TOP) {
        y = SCREEN_TOP - playerSize;
        velocityY = -velocityY * BOUNCE_FACTOR;
    }
    // Bottom wall
    if (y - playerSize < SCREEN_BOTTOM) {
        y = SCREEN_BOTTOM + playerSize;
        velocityY = -velocityY * BOUNCE_FACTOR;
    }
}

void Player::update() {
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
    if (std::abs(velocityX) < 0.0001f)
        velocityX = 0;
    if (std::abs(velocityY) < 0.0001f)
        velocityY = 0;

    // Handle rotation logic
    if (isRotating) {
        rotationAngle += rotationSpeed;
        if (rotationAngle >= 90.0f) {
            rotationAngle = 0.0f;  // Reset the angle
            isRotating = false;   // Stop the rotation
        }
    }

    // Interpolate head color back to orange
    float interpolationFactor = rotationAngle / 720.0f; // Slow transition
    for (int i = 0; i < 3; ++i) {
        currentHeadColor[i] = currentHeadColor[i] * (1.0f - interpolationFactor) + targetHeadColor[i] * interpolationFactor;
    }

    // Update particle system
    particleSystem.update(0.016f);  // Assuming 60 FPS, deltaTime = 1/60
    
    // Emit particles from the back of the ship
    float emitX = x;  // Adjust position based on ship size
    float emitY = y;
    particleSystem.emit(emitX, emitY + y_offset*2);
}

void Player::moveLeft() {
    velocityX -= acceleration;
    if (velocityX < -maxSpeed) velocityX = -maxSpeed;
}

void Player::moveRight() {
    velocityX += acceleration;
    if (velocityX > maxSpeed) velocityX = maxSpeed;
}

void Player::moveUp() {
    velocityY += acceleration;
    if (velocityY > maxSpeed) velocityY = maxSpeed;
}

void Player::moveDown() {
    velocityY -= acceleration;
    if (velocityY < -maxSpeed) velocityY = -maxSpeed;
}

// Getters
float Player::getX() const { return x; }
float Player::getY() const { return y; }
float Player::getSize() const { return playerSize; }

void Player::shoot(std::vector<Projectile*>& projectiles) {
    if (isRotating) {
        std::cout << "Cannot shoot while rotating!" << std::endl;
        return;  // Prevent shooting if the player is rotating
    }

    for (int i = 0; i < projectiles.size(); i++) {
        if (!projectiles[i]->isActive()) {
            projectiles[i]->spawn(x, y + playerSize);
            startRotation();  // Start rotation after shooting

            // Change head color to grey when shooting
            currentHeadColor[0] = 0.5f;
            currentHeadColor[1] = 0.5f;
            currentHeadColor[2] = 0.5f;

            break;
        }
    }
}

bool Player::checkCollision(float projectileX, float projectileY, float projectileWidth, float projectileHeight) {
    // Calculate player's bounding box
    float playerLeft = x - playerSize;
    float playerRight = x + playerSize;
    float playerTop = y + playerSize;
    float playerBottom = y - playerSize;

    // Calculate projectile's bounding box
    float projectileLeft = projectileX - projectileWidth/2;
    float projectileRight = projectileX + projectileWidth/2;
    float projectileTop = projectileY + projectileHeight/2;
    float projectileBottom = projectileY - projectileHeight/2;

    // Check for overlap
    bool collisionX = playerRight > projectileLeft && playerLeft < projectileRight;
    bool collisionY = playerTop > projectileBottom && playerBottom < projectileTop;

    return collisionX && collisionY;
}
